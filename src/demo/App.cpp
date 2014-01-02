#include "demo/App.h"

#include "demo/PlainSphere.h"

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
_boundTop(1), _boundBottom(-1), _boundLeft(-1), _boundRight(1),
_modelMat(), _viewMat(), _projMat(), _VAOs(nullptr), _VBOs(nullptr), _numVAO(0), _numVBO(0), _program(), _demos(), _currentDemo(-1),
_targetDemo(-1), _world(), _paused(false) {
  // do nothing
}

App::~App() {
  _program.del();
  clearObjects();
}

PlainSphere& App::newPlainSphere(const re::Ent& ent) {
  PlainSphere* s = new PlainSphere(ent);
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
    req.obj->gSetup(_VAOs[req.vaoIndex], _VBOs[req.vboIndex], _program);
  });
  
  _currentDemo = _targetDemo;
}

void App::gResizeScreen(int w, int h) {
  _screenHeight = (h > 0) ? h : 1;
  _screenWidth = w;
  
  glViewport(0, 0, _screenWidth, _screenHeight);
  
  _aspectRatio = _screenWidth / GLfloat(_screenHeight);
  
  GLfloat mid = (_boundLeft + _boundRight) / 2.0f;
  GLfloat left = _aspectRatio * (_boundLeft - mid) + mid;
  GLfloat right = _aspectRatio * (_boundRight - mid) + mid;
  
#ifdef USE_OLD_SYNTAX
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, _boundBottom, _boundTop, 1, 5);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#else
  _projMat.setIdentity();
  _projMat.frustum(left, right, _boundBottom, _boundTop, 1, 5);
  _program.setProjMat(_projMat);
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
  _cam.step();
  _program.setViewMat(_cam.viewMat());
  MatrixStack stack;
  stack.rotate(angle * PI / 180.0f, 0, 1, 0);
  for_each(_gfxObjs.begin(), _gfxObjs.end(), [&](GfxObj* obj) {
    obj->gDraw(stack, _program);
  });
#else
  glPushMatrix();
  
  MatrixStack stack;
  stack.rotate(angle * PI / 180.0f, 0, 1, 0);
  stack.push();
  stack.translate(0, 0, -2);
  mat4 m = stack.getMatrix();
  stack.pop();
  
  float tmp[4][4];
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      tmp[j][i] = m.value[i][j];
  
  glLoadMatrixf(&tmp[0][0]);
//  glTranslatef(0, 0, -2);
//  glRotatef(angle, 0, 1, 0);
  
  glBegin(GL_TRIANGLE_STRIP);//  int idx = 0;
  const int nSides = 20;
  const double radius = 1;
  for (int i = 0; i < nSides - 3; i++) {
    double a1 = PI * (i + 1.0) / (double)(nSides - 1);
    double a2 = PI * (i + 2.0) / (double)(nSides - 1);
    
    float r1 = radius * sin(a1);
    float r2 = radius * sin(a2);
    
    float z1 = radius * cos(a1);
    float z2 = radius * cos(a2);
    
    for (int j = 0; j <= nSides; j++) {
      double b = 2 * PI * (nSides - j) / (double)nSides;
      double c = cos(b);
      double s = sin(b);
      
      glVertex3f(r1*s, r1*c, z1);
      glNormal3f(r1*s, r1*c, z1);
      glColor4f(c, s, 1.0f, 0.9f);
      glVertex3f(r2*s, r2*c, z2);
      glNormal3f(r2*s, r2*c, z2);
      glColor4f(c, s, 1.0f, 0.9f);
    }
  }
  glEnd();
  
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 0, radius);
  glColor4f(0, 1, 1, 0.9f);
  glNormal3f(0, 0, 1);
  double a1 = PI * 1.0 / (double)(nSides - 1);
  float r1 = radius * sin(a1);
  float z1 = radius * cos(a1);
  for (int i = 0; i <= nSides; i++) {
    double b = 2 * PI * (nSides - i) / (double)nSides;
    double c = cos(b);
    double s = sin(b);
    glVertex3f(r1*s, r1*c, z1);
    glNormal3f(r1*s, r1*c, z1);
    glColor4f(c, s, 1.0f, 0.9f);
  }
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
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef USE_OLD_SYNTAX
  _program.init();
  gResizeScreen(_screenWidth, _screenHeight);
#else
  glShadeModel(GL_FLAT);
#endif
  
//  glEnable(GL_COLOR_LOGIC_OP);
//  glLogicOp(GL_XOR);
}

void App::switchToDemo(int index) {
  _targetDemo = index;
}

