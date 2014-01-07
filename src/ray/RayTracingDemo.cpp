#include "ray/RayTracingDemo.h"

#include "react/Entities/reEnt.h"
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
    printf("  %5.1f%% complete --- ", percentageCompleted);
    if (ms < 1000) {
      printf("%ld ms\n", ms);
    } else if (ms < 60 * 1000) {
      printf("%.3f s\n", ms/1000.0);
    } else if (ms < 3600 * 1000) {
      printf("%.2f mins\n", ms / 60000.0);
    }
  }
}

RayTracingDemo::RayTracingDemo() : DemoApp(), _world(), _maxDepth(5), _imageWidth(1), _imageHeight(1), _outputFile(), _fovy(45.0), _viewMat(1.0), _ambient(0.2f, 0.2f, 0.2f), _attenuation(1.0f, 0.0f, 0.0f), _lights(), _pixels(nullptr), _renderWidth(128), _renderHeight(96), _infinityColor(0.0, 0.0, 0.0), _sceneFile(), _lightNo(0), usingGL(false) {
  _sceneFile = "resources/ray/samples/scene4-emission.test";
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

/**
 * Recursively shoots rays at the image to determine the color
 */

reVector RayTracingDemo::shootRay(unsigned int depth, const reVector& origin, const reVector& dir) {
  // reached recursion limit
  if (depth >= _maxDepth) {
    return _infinityColor;
  }
  
  reVector intersect, norm;
  reEnt* ent = _world.shootRay(origin, dir, &intersect, &norm);
  
  // no more objects in this direction
  if (ent == nullptr) {
    return _infinityColor;
  }
  
  RayObject* obj = (RayObject*)ent->userdata;
  
  reVector color = _ambient + obj->emission();
  
  for_each(_lights.begin(), _lights.end(), [&](const RayLightSource* light) {
    intersect = _inverseViewMat.mult(intersect, 1.0);
    
    reVector ray;
    if (light->isDirectional()) {
      ray = -light->transformedVect();
    } else {
      ray = light->transformedVect() - intersect;
    }
    ray.normalize();
    
    // query using the ray
    reEnt* other = _world.shootRay(intersect, ray);
    
    // if query is successful
    if (other == nullptr) {
      norm = _inverseViewMat.mult(norm, 0.0).normalized();
      const reVector lightPos = light->transformedVect();
      const reVector back = (lightPos - intersect).normalized();
      const reVector halfVec = (back + ray).normalized();
      
      const reVector reflec = dir + norm * (-2 * dir.dot(norm));
      const reVector diffuse = obj->diffuse() * reMax(ray.dot(norm), 0.0f);
      const reVector specular = 
            obj->specular() * shootRay(depth + 1, intersect, reflec) +
            obj->specular() * rePow(reMax(norm.dot(halfVec), 0.0f), obj->shininess());
      
//      printf("SPEC=(%.2f, %.2f, %.2f)\n", specular[0], specular[1], specular[2]);
      
      if (light->isDirectional()) {
        color += light->color() * (diffuse + specular);
      } else {
        float dist = (light->vect() - intersect).length();
        color += light->color() * (diffuse + specular) / (_attenuation[0] + _attenuation[1] * dist + _attenuation[2] * dist * dist);
      }
    }
  });
  
  return color;
}

/**
 * Resize the image buffer
 */

void RayTracingDemo::resizeImage(GLsizei w, GLsizei h) {
  if (_pixels != nullptr) {
    delete[] _pixels;
  }
  
  const GLuint size = w * h * 4;
  _pixels = new GLubyte[size];
  memset(&_pixels[0], 150, sizeof(GLubyte) * size);
  
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
  
  for (auto& light : _lights) {
    if (light->isDirectional()) {
      light->transformedVect() = _inverseViewMat.mult(light->vect(), 0.0);
    } else {
      light->transformedVect() = _inverseViewMat.mult(light->vect(), 1.0);
    }
  }
  
  // precompute tan(x)
  float* tanx = new float[w];
  for (GLsizei i = 0; i < w; i++) {
    tanx[i] = glm::tan(fovx * (i + 0.5f - halfWidth) / halfWidth);
  }
  
  // setup the viewing origin
  
  // get useful information to update status
  const GLsizei numPixels = w * h;
  GLsizei numProcessed = 0;
  const long PERIOD = 1 * 1000;
  timeval start, lastChecked, now;
  gettimeofday(&start, nullptr);
  gettimeofday(&lastChecked, nullptr);
  
  const reVector eye = _inverseViewMat.mult(reVector(0,0,0), 1.0);

  for (GLsizei i = 0; i < h; i++) {
    float tany = glm::tan(fovy * (i + 0.5f - halfHeight) / halfHeight);
    
    for (GLsizei j = 0; j < w; j++) {
      const reVector ray = _inverseViewMat.mult(
        reVector(tanx[j], tany, -1.0), 0.0).normalized();
      
      colorPixel(&_pixels[4*i*w + 4*j], shootRay(0, eye, ray));
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
  
  // compute total time taken
  gettimeofday(&now, nullptr);
  statusUpdate(100.0, timeBetween(start, now));
  gettimeofday(&lastChecked, nullptr);
  
  printf("[INFO]  Rendering of %d x %d image complete\n", _renderWidth, _renderHeight);
  
  delete[] tanx;
}

