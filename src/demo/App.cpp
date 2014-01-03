#include "demo/App.h"

#include "demo/PlainSphere.h"
#include "demo/StaticGfx.h"

#include <GL/freeglut.h>
#include <cstdio>
#include <map>
#include <algorithm>

//#define USE_OLD_SYNTAX

using namespace demo;

template <typename K, typename V>
V GetWithDef(const  std::map <K,V> & m, const K & key, const V & defval ) {
   typename std::map<K,V>::const_iterator it = m.find( key );
   if ( it == m.end() ) {
      return defval;
   }
   else {
      return it->second;
   }
}

App::App() : _screenHeight(0), _screenWidth(0), _aspectRatio(0.0),
_frameTop(1), _frameBottom(-1), _frameLeft(-1), _frameRight(1),
_frameStart(1), _frameDepth(35), _modelMat(), _viewMat(1.0f),
_projMat(1.0f), _VAOs(nullptr), _VBOs(nullptr), _numVAO(0),
_numVBO(0), _canvas(), _demos(), _currentDemo(-1), _targetDemo(-1),
_world(), _paused(false) {
  // do nothing
}

App::~App() {
  _canvas.release();
  clearObjects();
}

PlainSphere& App::newPlainSphere(const re::Ent& ent) {
  PlainSphere* s = new PlainSphere(ent);
  add(*((GfxObj*)s));
  return *s;
}

StaticGfx& App::newStaticGfx() {
  StaticGfx* s = new StaticGfx();
  add(*((GfxObj*)s));
  return *s;
}

void App::add(GfxObj& obj) {
  obj.setApp(this);
  _gfxObjs.push_back(&obj);
}

void App::run() {
  glutMainLoop();
}

void App::keyEvent(unsigned char key, int, int) {
  switch (key) {
    case 27: // ESC
      glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
      glutLeaveMainLoop();
      break;
    
    case 32: // SPACE
      _paused = !_paused;
      if (!_paused) {
        glutPostRedisplay();
      }
      break;
  }
}

#define PUSH  1.0f
#define TWIRL 0.4f

void App::specialKeyEvent(int key, int, int) {
  int modifiers = glutGetModifiers();
  switch (key) {
    case GLUT_KEY_UP:
      if (modifiers & GLUT_ACTIVE_ALT) {
        _cam.rotUp(TWIRL);
      } else {
        _cam.pushForward(PUSH);
      }
      break;
    
    case GLUT_KEY_LEFT:
      if (modifiers & GLUT_ACTIVE_ALT) {
        _cam.rotLeft(TWIRL);
      } else {
        _cam.pushLeft(PUSH);
      }
      break;
      
    case GLUT_KEY_RIGHT:
      if (modifiers & GLUT_ACTIVE_ALT) {
        _cam.rotRight(TWIRL);
      } else {
        _cam.pushRight(PUSH);
      }
      break;
    
    case GLUT_KEY_DOWN:
      if (modifiers & GLUT_ACTIVE_ALT) {
        _cam.rotDown(TWIRL);
      } else {
        _cam.pushBack(PUSH);
      }
      break;
  }
}

void App::clearObjects() {
  for_each(_gfxObjs.begin(), _gfxObjs.end(), [](GfxObj* obj) {
    delete obj;
  });
  _gfxObjs.clear();
  
//  glDeleteTextures(_numTBO, _TBOs);
  glDeleteVertexArrays(_numVAO, _VAOs);
//  delete[] _TBOs;
  delete[] _VAOs;
  delete[] _VBOs;
  
  _VAOs = nullptr;
  _VBOs = nullptr;
}

void App::gStartDemo() {
  _world.clear();
  clearObjects();
  
  _demos.at(_targetDemo)(_world, *this);
  
  _cam.reset();
  
  printf("[DEMO]  Starting new demo %d\n", _targetDemo);
  
  // define temporary request objects
  std::vector<Request> requests;
  
  int numVAO(0), numVBO(0);//, numTBO(0);
  std::map<GfxObj::Type, bool> processed;
  for_each(_gfxObjs.begin(), _gfxObjs.end(), [&](GfxObj* obj) {
    if (!GetWithDef(processed, obj->type(), false)) {
//      numTBO += obj->numTBOReq();
      // set request object
      Request req;
      req.obj = obj;
      
      // set VAO index if requested
      if (obj->numVAOReq() > 0) {
        req.vaoIndex = numVAO;
        numVAO += obj->numVAOReq();
      }
      
      // set VBO index if requested
      if (obj->numVBOReq() > 0) {
        req.vboIndex = numVBO;
        numVBO += obj->numVBOReq();
      }
      
      requests.push_back(req);
      processed[obj->type()] = !obj->isDynamic();
    }
  });
  
  _numVAO = numVAO;
  _numVBO = numVBO;
//  _numTBO = numTBO;
  _VAOs = new GLuint[numVAO];
  _VBOs = new GLuint[numVBO];
//  _TBOs = new GLuint[numTBO];
  
  glGenBuffers(numVBO, _VBOs);
//  glGenTextures(numTBO, _TBOs);
  glGenVertexArrays(numVAO, _VAOs);
  
  for_each(requests.begin(), requests.end(), [&](Request req) {
    req.obj->setup(&_VAOs[req.vaoIndex], &_VBOs[req.vboIndex], _canvas);
  });
  
  _currentDemo = _targetDemo;
}

void App::gResizeScreen(int w, int h) {
  _screenHeight = (h > 0) ? h : 1;
  _screenWidth = w;
  
  glViewport(0, 0, _screenWidth, _screenHeight);
  
  _aspectRatio = _screenWidth / GLfloat(_screenHeight);
  
  GLfloat mid = (_frameLeft + _frameRight) / 2.0f;
  GLfloat left = _aspectRatio * (_frameLeft - mid) + mid;
  GLfloat right = _aspectRatio * (_frameRight - mid) + mid;
  
#ifdef USE_OLD_SYNTAX
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, _frameBottom, _frameTop, _frameStart, _frameDepth);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#else
  _canvas.frustum(left, right, _frameBottom, _frameTop, _frameStart, _frameDepth);
#endif
}

void App::gPaint() {
  if (_currentDemo != _targetDemo) {
#ifndef USE_OLD_SYNTAX
    gStartDemo();
#endif
  }
  
  _world.step(0.1);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  static float angle = 90;
  
#ifndef USE_OLD_SYNTAX
  _canvas.clearStack();
  _cam.step();
  _canvas.setViewMat(_cam.viewMat());
  _canvas.push();
  _canvas.rotate(angle, 0, 1, 0);
  for_each(_gfxObjs.begin(), _gfxObjs.end(), [&](GfxObj* obj) {
    obj->draw(_canvas);
  });
//  mprint(_canvas.modelMat());
//  printf("%f\n\n", angle);
  _canvas.pop();
#else
  glPushMatrix();
  glTranslatef(0, 0, -2);
  glRotatef(angle, 0, 1, 0);
  
  glBegin(GL_TRIANGLE_STRIP);//  int idx = 0;
  const int nSides = 20;
  const double radius = 1;
  
  double s[nSides+1];
  double c[nSides+1];
  double r[nSides+1];
  double z[nSides+1];
  for (int i = 0; i <= nSides; i++) {
    double a = 2 * PI * i / (double)nSides;
    double b = a / 2;
    s[i] = sin(a);
    c[i] = cos(a);
    r[i] = radius * sin(b);
    z[i] = radius * cos(b);
  }
  
  for (int i = 0; i < nSides; i++) {
    for (int j = 0; j < nSides; j++) {
      for (int k = 0; k < 2; k++) {
        const int ik = i + k;
        const int jk = j + k;
        const int ix = (i % 2 == 0) ? 1 : -1;
        glVertex3f(r[jk]*s[ik], r[jk]*c[ik], z[jk]*ix);
        glNormal3f(r[jk]*s[ik], r[jk]*c[ik], z[jk]*ix);
        glColor4f(c[ik], s[ik], 1.0f, 0.9f);
      }
    }
  }
  glEnd();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0, -1, 0);
  float m = 25.0f;
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(m, 0, m);
  glNormal3f(0, 1, 0);
  glColor3f(0.6, 0.1, 0.0);
  glVertex3f(-m, 0, m);
  glNormal3f(0, 1, 0);
  glColor3f(0.6, 0.1, 0.0);
  glVertex3f(m, 0, -m);
  glNormal3f(0, 1, 0);
  glColor3f(0.6, 0.1, 0.0);
  glVertex3f(-m, 0, -m);
  glNormal3f(0, 1, 0);
  glColor3f(0.6, 0.1, 0.0);
  glEnd();
  glPopMatrix();
#endif

  angle += 0.03f;
  
  glutSwapBuffers();
  
  if (!_paused) {
    glutPostRedisplay();
  }
}

void App::gInit() {
  printf("[DEMO]  Initializing demo graphics engine\n");
  
  int verMajor, verMinor;
  glGetIntegerv(GL_MAJOR_VERSION, &verMajor);
  glGetIntegerv(GL_MINOR_VERSION, &verMinor);
  printf("[DEMO]  Detected OpenGL version %d.%d\n", verMajor, verMinor);
  
  int multisamplingEnabled;
  glGetIntegerv(GL_SAMPLE_BUFFERS, &multisamplingEnabled);
  if (multisamplingEnabled == GL_TRUE) {
    printf("[DEMO]  Multisampling support detected\n");
  } else {
    printf("[DEMO]  Multisampling not supported on device\n");
  }
  
  glClearColor(0.3f, 0.3f, 0.3f, 0.5f);
  glLineWidth(1);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  glPolygonMode(GL_BACK, GL_FILL);
  glPolygonMode(GL_FRONT, GL_LINE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef USE_OLD_SYNTAX
  _canvas.init();
  gResizeScreen(_screenWidth, _screenHeight);
#else
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  float lightPos[] = { 0, 1.5, 0 };
  float lightColor[] = { 0.3, 0.3, 0.3, 0.3 };
//  float diffuse[] = { 0.5, 0.5, 0.5, 0.3 };
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
//  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  
  glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
#endif
  
//  glEnable(GL_COLOR_LOGIC_OP);
//  glLogicOp(GL_XOR);
}

void App::switchToDemo(int index) {
  _targetDemo = index;
}

