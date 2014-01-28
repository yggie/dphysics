#include "ray/RayTracingDemo.h"

#include "react/Entities/reEnt.h"
#include "react/Entities/reRigidBody.h"
#include "react/Collision/reBroadPhase.h"
#include "ray/RayObject.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cstring>
#include <sys/time.h>

namespace {
  long timeBetween(timeval& start, timeval& end) {
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    return 1000 * seconds + useconds / 1000;
  }
  
  void statusUpdate(float percentageCompleted, long ms) {
    printf("%11.1f%%%12d", percentageCompleted, re::queriesMade);
    if (ms < 1000) {
      printf("%12ld ms\n", ms);
    } else if (ms < 60 * 1000) {
      printf("%12.3f s\n", ms/1000.0);
    } else if (ms < 3600 * 1000) {
      printf("%12.2f mins\n", ms / 60000.0);
    }
  }
  
  const re::vec clamp(const re::vec& v) {
    re::vec a;
    for (int i = 0; i < 3; i++) {
      a[i] = reClamp(v[i], 0.0, 1.0);
    }
    return a;
  }
}

RayTracingDemo::RayTracingDemo() : DemoApp(), _world(), _maxDepth(5), _imageWidth(1), _imageHeight(1), _outputFile(), _fovy(45.0), _viewMat(1.0), _ambient(0.2f, 0.2f, 0.2f), _attenuation(1.0f, 0.0f, 0.0f), _lights(), _pixels(nullptr), _renderWidth(128), _renderHeight(96), _infinityColor(0.0, 0.0, 0.0), _sceneFile(), _lightNo(0), usingGL(false) {
  _sceneFile = "resources/ray/samples/scene5.test";
}

RayTracingDemo::~RayTracingDemo() {
  release();
}

void RayTracingDemo::init() {
  glClearColor(0, 0, 0, 1);
  glClearDepth(1.0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferi(GL_RED_SCALE, 1);
	glPixelTransferi(GL_RED_BIAS, 0);
	glPixelTransferi(GL_GREEN_SCALE, 1);
	glPixelTransferi(GL_GREEN_BIAS, 0);
	glPixelTransferi(GL_BLUE_SCALE, 1);
	glPixelTransferi(GL_BLUE_BIAS, 0);
	glPixelTransferi(GL_ALPHA_SCALE, 1);
	glPixelTransferi(GL_ALPHA_BIAS, 0);
	glShadeModel(GL_FLAT);
  
  restart();
}

void RayTracingDemo::restart() {
  release();
  createSceneFromFile(_sceneFile.c_str(), false);
  
  renderScene(64, 48);
}

void RayTracingDemo::release() {
  for (reEnt& ent : _world.entities()) {
    if (ent.userdata != nullptr) {
      delete (RayObject*)(ent.userdata);
      ent.userdata = nullptr;
    }
  }
  
  _world.clear();
  for_each(_lights.begin(), _lights.end(), [&](RayLightSource* light) {
    delete light;
  });
  _lights.clear();
  
  if (_pixels != nullptr) {
    delete[] _pixels;
  }
}

void RayTracingDemo::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if (_pixels != nullptr && !usingGL) {
    glDrawPixels(_renderWidth, _renderHeight, GL_RGBA, GL_UNSIGNED_BYTE, &_pixels[0]);
    checkOpenGLError();
  }
  
  glFlush();
}

void RayTracingDemo::onResize() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, _renderWidth, 0, _renderHeight, 1.0f, 5.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRasterPos3f(0.0f, 0.0f, -2.0f);
  glPixelZoom(width()/double(_renderWidth), height()/double(_renderHeight));
  glutPostRedisplay();
}

void RayTracingDemo::keyEvent(unsigned char key, int, int) {
  switch (key) {
    case 'a':
      renderScene(_imageWidth / 10, _imageHeight / 10);
      break;
      
    case 's':
      renderScene(_imageWidth / 5, _imageHeight / 5);
      break;
      
    case 'd':
      renderScene(_imageWidth / 2, _imageHeight / 2);
      break;
      
    case 'f':
      renderScene(_imageWidth, _imageHeight);
      break;
      
    case 'q':
      createSceneFromFile(_sceneFile.c_str(), true);
      break;
  }
}

void RayTracingDemo::specialKeyEvent(int, int, int) {
  // do nothing
}

void RayTracingDemo::mouseEvent(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        {
          const int X = (int)(_renderWidth * x / (float)width());
          const int Y = (int)(_renderHeight * y / (float)height());
          const GLubyte* pix = &_pixels[4*(_renderHeight - Y - 1)*_renderWidth + 4*X];
          printf("At (%3d, %3d) : #%02x%02x%02x%02x        windows size = (%3d, %3d)\n", x, y, pix[0], pix[1], pix[2], pix[3], width(), height());
        }
        break;
      
      case GLUT_RIGHT_BUTTON:
        break;
      
      case GLUT_MIDDLE_BUTTON:
        break;
    }
  } else if (state == GLUT_UP) {
    // do nothing
  } else {
    printf("Unknown button state %d\n", state);
  }
}

/**
 * Recursively shoots rays at the image to determine the color
 */

const re::vec RayTracingDemo::shootRay(unsigned int depth, const re::vec& origin, const re::vec& dir) {
  // reached recursion limit
  if (depth >= _maxDepth) {
    return _infinityColor;
  }
  
  re::vec intersect, norm;
  // primary ray
  const reEnt* ent = _world.queryWithRay(origin, dir, &intersect, &norm);
  
  // no more objects in this direction
  if (ent == nullptr) {
    return _infinityColor;
  }
  
  // get the material object associated with the entity
  const RayObject* obj = (RayObject*)ent->userdata;
  
  // initialize color with emission and ambient components
  re::vec color = clamp(_ambient + obj->emission());
  
  // determine lighting contributions
  for (auto it = _lights.begin(); it != _lights.end(); it++) {
    const RayLightSource* light = *it;
    
    // determine direction of shadow rays
    re::vec ray;
    if (light->isDirectional()) {
      ray = re::normalize(-light->vect());
    } else {
      ray = re::normalize(light->vect() - intersect);
    }
    
    // trace shadow rays to light sources
    re::vec objIntersect;
    reEnt* other = _world.queryWithRay(intersect, ray, &objIntersect);
    
    // if query is successful
    if (other == nullptr || (!light->isDirectional() && re::lengthSq(objIntersect - intersect) > re::lengthSq(light->vect() - intersect))) {
      const re::vec back = re::normalize(light->vect() - intersect);
      const re::vec halfVec = re::normalize(back + ray);
      
      const re::vec diffuse = clamp(obj->diffuse() * reMax(re::dot(ray, norm), 0.0f));
      const re::vec specular = clamp(obj->specular() * rePow(reMax(re::dot(norm, halfVec), 0.0f), obj->shininess()));
      
//      printf("SPECULAR: (%.2f, %.2f, %.2f)\n", specular[0], specular[1], specular[2]);
      
      if (light->isDirectional()) {
        color += clamp(light->color() * (diffuse + specular));
      } else {
        const float dist = re::length(light->vect() - intersect);
        color += clamp(light->color() * (diffuse + specular) / (_attenuation[0] + _attenuation[1] * dist + _attenuation[2] * dist * dist));
      }
    }
  };
  
  // TODO reflections don't seem to match samples in the discussion
  if (re::length(obj->specular()) > RE_FP_TOLERANCE) {
    const re::vec reflec = re::normalize(dir - norm * 2.0 * re::dot(norm, dir));
    // shoot secondary rays
    color += clamp(obj->specular() * shootRay(depth + 1, intersect, reflec));
  }
  
  return color;
}

/**
 * Resize the image buffer
 */

void RayTracingDemo::resizeImage(GLsizei w, GLsizei h) {
  if (_pixels != nullptr) {
    delete[] _pixels;
    _pixels = nullptr;
  }
  
  const GLuint size = w * h * 4;
  _pixels = new GLubyte[size];
  memset(&_pixels[0], 123, sizeof(GLubyte) * size);
  
  _renderWidth = w;
  _renderHeight = h;
  onResize();
}

/**
 * Renders the scene with the specified rendering sizes using ray tracing
 */

void RayTracingDemo::renderScene(GLsizei w, GLsizei h) {
  usingGL = false;
  resizeImage(w, h);
  const float aspectRatio = w / float(h);
  const float fovy = glm::radians(_fovy / 2.0);
  const float fovx = aspectRatio * fovy;
  
  const float halfWidth = w / 2.0f;
  const float halfHeight = h / 2.0f;
  
  // precompute tan(x)
  float* tanx = new float[w];
  for (GLsizei i = 0; i < w; i++) {
    tanx[i] = glm::tan(fovx * (i + 0.5f - halfWidth) / halfWidth);
  }
  
  // get useful information to update status
  const GLsizei numPixels = w * h;
  GLsizei numProcessed = 0;
  const long PERIOD = 1 * 1000;
  
  // START TIMER
  timeval start, lastChecked, now;
  gettimeofday(&start, nullptr);
  gettimeofday(&lastChecked, nullptr);
  
  const re::vec eye = _inverseViewMat.mult(re::vec(0,0,0), 1.0);
  
  _world.update();
  reBPMeasure m;
  _world.broadPhase().measure(m);
  printf("[INFO]   B-Phase: (refRatio=%.2f, child=%d, leafs=%d, meanDepth=%.1f)\n", m.references/(float)m.entities, m.children, m.leafs, m.meanDepth);
  
  printf("[INFO]   Image size: %d x %d\n", _renderWidth, _renderHeight);
  printf("%12s%12s%12s\n", "% complete", "Queries", "Time");
  printf("---------------------------------------\n");
  statusUpdate(0.0, 0);

  for (GLsizei i = 0; i < h; i++) {
    const float tany = glm::tan(fovy * (i + 0.5f - halfHeight) / halfHeight);
    
    for (GLsizei j = 0; j < w; j++) {
      const re::vec ray = _inverseViewMat.mult(
        re::normalize(re::vec(tanx[j], -tany, -1.0)), 0.0);
      
      // shoot primary ray
      const re::vec color = shootRay(0, eye, ray);
      const int IDX = 4*(h - i - 1)*w + 4*j;
      GLubyte* pix = &_pixels[IDX];
      pix[0] = (GLubyte)(255.0 * reClamp(color[0], 0.0, 1.0));
      pix[1] = (GLubyte)(255.0 * reClamp(color[1], 0.0, 1.0));
      pix[2] = (GLubyte)(255.0 * reClamp(color[2], 0.0, 1.0));
      pix[3] = 0xff;
      RE_ASSERT_MSG((IDX < 4*w*h-3 && IDX >= 0), "Buffer overflow detected\n")
//      colorPixel(&_pixels[4*(h - i - 1)*w + 4*j], shootRay(0, eye, ray));
    }
    
    // give useful feedback to the user after a certain period
    numProcessed += w;
    gettimeofday(&now, nullptr);
    if (timeBetween(lastChecked, now) > PERIOD) {
      statusUpdate(100.0 * numProcessed/double(numPixels),
                   timeBetween(start, now));
      draw();
      gettimeofday(&lastChecked, nullptr);
    }
  }
  
  // STOP TIMER
  gettimeofday(&now, nullptr);
  statusUpdate(100.0, timeBetween(start, now));
  gettimeofday(&lastChecked, nullptr);
  printf("---------------------------------------\n");
  
  re::queriesMade = 0;
  
  delete[] tanx;
}

