#ifndef DEMO_DEMOAPP_H
#define DEMO_DEMOAPP_H

/**
 * The base class which all demo applications should extend
 */

class DemoApp {
public:
  DemoApp();
  virtual ~DemoApp();
  
  virtual void init() = 0;
  virtual void restart() = 0;
  virtual void release() = 0;
  virtual void draw() = 0;
  virtual void onResize() = 0;
  virtual void keyEvent(unsigned char key, int x, int y) = 0;
  virtual void specialKeyEvent(int key, int x, int y) = 0;
  virtual void mouseEvent(int button, int state, int x, int y) = 0;
  
  unsigned int width() const;
  unsigned int height() const;
  void setWindowSize(unsigned int w, unsigned int h);
  
protected:
  unsigned int _windowWidth;
  unsigned int _windowHeight;
};

inline DemoApp::DemoApp() : _windowWidth(1), _windowHeight(1) {
  // do nothing
}

inline DemoApp::~DemoApp() {
  // do nothing
}

inline unsigned int DemoApp::width() const {
  return _windowWidth;
}

inline unsigned int DemoApp::height() const {
  return _windowHeight;
}

inline void DemoApp::setWindowSize(unsigned int w, unsigned int h) {
  _windowWidth = w;
  _windowHeight = h;
}

#endif
