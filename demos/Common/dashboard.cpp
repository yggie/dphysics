#include "dashboard.h"

#include "App.h"
#include "glsetup.h"

#include "../RayTracing/RayTracingDemo.h"

namespace {
  re::demo::App* demos[] = {
    new RayTracingDemo(),
  };
  
  re::demo::App::Options options;

  const unsigned int NUM_DEMOS = sizeof(demos) / sizeof(re::demo::App*);
  
  unsigned int currentDemo = 0;
  unsigned int winWidth = 0;
  unsigned int winHeight = 0;
  int lastX = -1;
  int lastY = -1;
  
  /**
   * @ingroup demo
   * Convenient function to reference the current demo
   */
  
  re::demo::App& demo() {
    return *demos[currentDemo];
  }
  
  /**
   * @ingroup demo
   * Called when the application requests to change the demo
   */

  void changeDemo(unsigned int index) {
    currentDemo = index;
    demo().init();
    options = demo().options();
  }
  
  /**
   * @ingroup demo
   * Initializes the first demo
   */

  void initFunc() {
    int verMajor, verMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &verMajor);
    glGetIntegerv(GL_MINOR_VERSION, &verMinor);
    printf("[DEMO]   Detected OpenGL version %d.%d\n", verMajor, verMinor);
    
    int multisamplingEnabled;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &multisamplingEnabled);
    if (multisamplingEnabled == GL_TRUE) {
      printf("[DEMO]   Multisampling support detected\n");
    } else {
      printf("[DEMO]   Multisampling NOT supported on this device\n");
    }
    
    changeDemo(currentDemo);
  }
  
  /**
   * @ingroup demo
   * Propagates the drawing call to the current running demo
   */

  void displayFunc() {
    demo().draw();
  }
  
  /**
   * @ingroup demo
   * Propagates the key event call to the current running demo
   */

  void keyFunc(unsigned char key, int x, int y) {
    switch (key) {
      case 27:  // ESC
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                      GLUT_ACTION_CONTINUE_EXECUTION);
        glutLeaveMainLoop();
        break;
        
      default:
        demo().keyEvent(key, x, y);
        break;
    }
  }
  
  /**
   * @ingroup demo
   * Propagates the special key event to the current running demo
   */

  void specialKeyFunc(int key, int x, int y) {
    demo().specialKeyEvent(key, x, y);
  }
  
  /**
   * @ingroup demo
   * Propagates the mouse button event to the current running demo
   */

  void mouseFunc(int button, int state, int x, int y) {
    demo().mouseEvent(button, state, x, y);
  }
  
  /**
   * @ingroup demo
   * Propagates the mouse motion event to the current running demo
   */
  
  void generalMotionEvent(bool passive, int x, int y) {
    if (lastX == -1 && lastY == -1) {
      lastX = x;
      lastY = y;
      return;
    }
    
    if (passive) {
      demo().passiveMotionEvent(
        (x - lastX)/float(winWidth/2),
        (y - lastY)/float(winHeight/2)
      );
    } else {
      demo().motionEvent(
        (x - lastX)/float(winWidth/2),
        (y - lastY)/float(winHeight/2)
      );
    }
    lastX = x;
    lastY = y;
    
    if (options.trapMouse && (fabs(winWidth/2 - x) > 25 || fabs(winHeight/2 - y) > 25)) {
      glutWarpPointer(winWidth/2, winHeight/2);
      lastX = winWidth/2;
      lastY = winHeight/2;
    }
  }
  
  /**
   * @ingroup demo
   * Passes the function to the more generic generalMotionEvent function
   */
  
  void motionFunc(int x, int y) {
    generalMotionEvent(false, x, y);
  }
  
  /**
   * @ingroup demo
   * Passes the function to the more generic generalMotionEvent function
   */
  
  void passiveMotionFunc(int x, int y) {
    generalMotionEvent(true, x, y);
  }
  
  /**
   * @ingroup demo
   * Called when the window resizes
   */

  void reshapeFunc(int w, int h) {
    winHeight = (h > 0) ? h : 1;
    winWidth = w;
    lastX = -1;
    lastY = -1;
    glViewport(0, 0, winWidth, winHeight);
    demo().setWindowSize(winWidth, winHeight);
    demo().onResize(winWidth, winHeight);
  }
}

int startDemo(int argc, char** argv, DemoID demoNum) {
  currentDemo = demoNum;
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(200, 150);
  glutCreateWindow("react Physics Engine -- Demos");
  
  // register all the callback functions to be used
  glutReshapeFunc(reshapeFunc);
  glutKeyboardFunc(keyFunc);
  glutSpecialFunc(specialKeyFunc);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutPassiveMotionFunc(passiveMotionFunc);
  
  // run initialization function
  glutDisplayFunc(initFunc);
  glutMainLoopEvent();
  
  // register display function and begin main loop
  glutDisplayFunc(displayFunc);
  glutMainLoop();

  // releases all resources used by the demo applications
  for (unsigned int i = 0; i < NUM_DEMOS; i++) {
    delete demos[i];
    demos[i] = nullptr;
  }
  
  return 0;
}

