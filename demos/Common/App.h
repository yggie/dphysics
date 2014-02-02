#ifndef DEMO_App_H
#define DEMO_App_H

/**
 * The base class which all demo applications should extend
 */

namespace re {
  namespace demo {
    
    class App {
    public:
      
      struct Options {
        bool trapMouse;
      };
      
      App();
      virtual ~App();
      
      virtual void init() = 0;
      virtual void restart() = 0;
      virtual void release() = 0;
      virtual void draw() = 0;
      virtual void onResize(unsigned int w, unsigned int h) = 0;
      virtual void keyEvent(unsigned char, int, int) { } // stub
      virtual void specialKeyEvent(int, int, int) { }
      virtual void mouseEvent(int, int, int, int) { } // stub
      virtual void motionEvent(float, float) { }  // stub
      virtual void passiveMotionEvent(float, float) { } // stub
      virtual Options options() const = 0;
      
      unsigned int width() const;
      unsigned int height() const;
      void setWindowSize(unsigned int w, unsigned int h);
      
    protected:
      unsigned int _windowWidth;
      unsigned int _windowHeight;
    };

    inline App::App() : _windowWidth(1), _windowHeight(1) {
      // do nothing
    }

    inline App::~App() {
      // do nothing
    }

    inline unsigned int App::width() const {
      return _windowWidth;
    }

    inline unsigned int App::height() const {
      return _windowHeight;
    }

    inline void App::setWindowSize(unsigned int w, unsigned int h) {
      _windowWidth = w;
      _windowHeight = h;
    }
  }
}

#endif
