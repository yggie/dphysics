#ifndef DEMO_APPWINDOW_H
#define DEMO_APPWINDOW_H

#include "react/Ent.h"
#include "react/World.h"
#include "demo/MatrixStack.h"
#include "demo/MyProgram.h"
#include "demo/FlyingCam.h"

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <string>
#include <vector>

namespace demo { class App; }

typedef void(*Demo)(re::World& world, demo::App& app);

namespace demo {
  
  class GfxObj;
  class PlainSphere;
  
  /**
   * @ingroup demo
   * The base class for GUI demo apps
   */
   
  class App {
  public:
    App();
    ~App();
    
    PlainSphere& newPlainSphere(const re::Ent& ent);
    
    void add(GfxObj& obj);
    void init(int argc, char** argv);
    void run();
    
    void keyEvent(unsigned char key, int x, int y);
    void specialKeyEvent(int key, int x, int y);
    
    void clearObjects();
    
    void gResizeScreen(int w, int h);
    void gPaint();
    void gInit();
    
    void addDemo(Demo demo);
    
  private:
    void nextDemo();
    void switchToDemo(int index);
    void gStartDemo();
    
    int _screenHeight;
    int _screenWidth;
    float _aspectRatio;
    float _boundTop;
    float _boundBottom;
    float _boundLeft;
    float _boundRight;
    MatrixStack _modelMat;
    mat4 _viewMat;
    mat4 _projMat;
    
    GLuint* _VAOs;
    GLuint* _VBOs;
//    GLuint* _TBOs;
    
    int _numVAO;
    int _numVBO;
//    int _numTBO;
    
    MyProgram _program;
    
    std::vector<GfxObj*> _gfxObjs;
    
    std::vector<Demo> _demos;
    int _currentDemo;
    int _targetDemo;
    
    re::World _world;
    
    bool _paused;
    
    FlyingCam _cam;
  };
  
  inline void App::addDemo(Demo demo) {
    if (_demos.size() == 0) {
      _targetDemo = 0;
    }
    _demos.push_back(demo);
  }
  
  inline void App::nextDemo() {
    _currentDemo = (_currentDemo + 1) % _demos.size();
    switchToDemo(_currentDemo);
  }
  
  /**
   * @ingroup demo
   * Used to internally manage VAO, VBO and TBO requests in function
   * App::gInit()
   */

  struct Request {
    Request() : vaoIndex(-1), vboIndex(-1), tboIndex(-1), obj(nullptr) { }
    int vaoIndex;
    int vboIndex;
    int tboIndex;
    GfxObj* obj;
  };
}

#endif
