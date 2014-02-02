#ifndef DEMO_VIEWCAM_H
#define DEMO_VIEWCAM_H

#include "react/math.h"

namespace re {
  namespace demo {
    class ViewCam {
    public:
      enum Mode {
        FIRST_PERSON,
        SKY_VIEW,
        FOLLOWING
      };
      
      ViewCam();
      ~ViewCam();
      
      void init(Mode mode);
      void update();
      void reset();
      
      const re::mat4 viewMat() const;
      
      void keyEvent(unsigned char key, int x, int y);
      void specialKeyEvent(int key, int x, int y);
      void mouseEvent(int button, int state, int x, int y);
      void motionEvent(float dx, float dy);
      void passiveMotionEvent(float dx, float dy);
      
    private:
      re::mat4 _view;
      re::vec3 _facing;
      Mode _mode;
      
      re::mat3 _rot;
      re::vec3 _translation;
      re::vec3 _target;
      re::vec3 _faceTarget;
      re::vec3 _rota;
      re::vec3 _rotTarget;
    };
  }
}

#endif
