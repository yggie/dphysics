#ifndef PLANETARY_MOTION_DEMO_H
#define PLANETARY_MOTION_DEMO_H

#include "demos/Common/App.h"
#include "demos/Common/ViewCam.h"
#include "demos/Common/SimpleCanvas.h"

#include "react/reWorld.h"

namespace re {
  namespace demo {
    class PlanetaryMotionDemo : public App {
    public:
      PlanetaryMotionDemo();
      ~PlanetaryMotionDemo();
      
      void init() override;
      void restart() override;
      void release() override;
      void draw() override;
      void onResize(unsigned int w, unsigned int h) override;
      App::Options options() const override;
      
      void keyEvent(unsigned char key, int x, int y) { _cam.keyEvent(key, x, y); }
      void specialKeyEvent(int key, int x, int y) { _cam.specialKeyEvent(key, x, y); }
      void mouseEvent(int button, int state, int x, int y) { _cam.mouseEvent(button, state, x, y); }
      void motionEvent(float dx, float dy) { _cam.motionEvent(dx, dy); }
      void passiveMotionEvent(float dx, float dy) { _cam.passiveMotionEvent(dx, dy); }
    
    private:
      void prepareWorld();
      
      reWorld _world;
      ViewCam _cam;
      SimpleCanvas _canvas;
    };
    
    inline App::Options PlanetaryMotionDemo::options() const {
      re::demo::App::Options opt;
      opt.trapMouse = true;
      opt.dynamic = true;
      return opt;
    }
  }
}

#endif
