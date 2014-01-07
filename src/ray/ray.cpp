#include "demo/glsetup.h"
#include "demo/DemoApp.h"
#include "ray/RayTracingDemo.h"

#include "react/react.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

// declare all demos
DemoApp* demos[] = {
  nullptr,
};


const int numDemos = sizeof(demos) / sizeof(DemoApp*);
int currentDemo = 0;
unsigned int windowWidth = 0;
unsigned int windowHeight = 0;

void changeDemo(unsigned int index) {
  currentDemo = index;
  demos[currentDemo]->init();
}

void initFunc() {
  changeDemo(0);
}

void displayFunc() {
  demos[currentDemo]->draw();
}

void keyFunc(unsigned char key, int x, int y) {
  switch (key) {
    case 27:  // ESC
      glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_CONTINUE_EXECUTION);
      glutLeaveMainLoop();
      break;
      
    default:
      demos[currentDemo]->keyEvent(key, x, y);
      break;
  }
}

void specialKeyFunc(int key, int x, int y) {
  demos[currentDemo]->specialKeyEvent(key, x, y);
}

void reshapeFunc(int w, int h) {
  windowHeight = (h > 0) ? h : 1;
  windowWidth = w;
  glViewport(0, 0, windowWidth, windowHeight);
  demos[currentDemo]->setWindowSize(windowWidth, windowHeight);
  demos[currentDemo]->onResize();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(200, 150);
  glutCreateWindow("react Demo Applications");
  
  const reUInt SZ = 1024*1024*1;
  char* buffer = new char[SZ];
  reFreeListAllocator* tmp = new reFreeListAllocator(SZ, &buffer[0]);
  re::globalAllocator = new reProxyAllocator(tmp);
  
  demos[0] = new RayTracingDemo();
  
  glutReshapeFunc(reshapeFunc);
  glutKeyboardFunc(keyFunc);
  glutSpecialFunc(specialKeyFunc);
  
  glutDisplayFunc(initFunc);
  glutMainLoopEvent();
  glutDisplayFunc(displayFunc);
    
  glutMainLoop();
  
//  reAABB aa, bb;
//  reVector a(0,0,0), b(2,5,0);
//  
//  aa.dimens() = reVector(1,10,1);
//  bb.dimens() = reVector(1,1,1);
//  
//  if (aa.intersects(bb, a - b)) {
//    printf("SHOOOOOOOOOES\n");
//  } else {
//    printf("WKKAKAKAKAKAK\n");
//    printf("WKKAKAKAKAKAK\n");
//    printf("WKKAKAKAKAKAK\n");
//    printf("WKKAKAKAKAKAK\n");
//    printf("WKKAKAKAKAKAK\n");
//    printf("WKKAKAKAKAKAK\n");
//  }
  
//  reWorld world;
//  world.newRigidBody().withShape(
//    reTriangle(
//      reVector(-1.0, -0.2,  1.4),
//      reVector( 1.0,  1.4,  0.2),
//      reVector(-1.0,  1.4,  0.2)
//    )
//  ).at(0, 0, 0);
//  
//  reVector origin(1.1, 1.1, 0);
//  reVector dir(0, 0, -1);
//  reVector intersect, norm;
//  if (world.shootRay(origin, dir, &intersect, &norm)) {
//    printf("INTERSECT: (%+.2f, %+.2f, %+.2f)\n", intersect[0], intersect[1], intersect[2]);
//    printf("NORMAL:    (%+.2f, %+.2f, %+.2f)\n", norm[0], norm[1], norm[2]);
//  } else {
//    printf("NO INTERSECT\n");
//  }

  for (int i = 0; i < numDemos; i++) delete demos[i];
  
  RE_LOG("SUCCESSFULLY DELETED ALL DEMOS")
  
  delete re::globalAllocator;
  delete tmp;
  
  return 0;
}
