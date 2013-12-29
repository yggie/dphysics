#include <GL/freeglut.h>
#include "vec.h"
#include "debug.h"

//static void paint() {
//  glClear(GL_COLOR_BUFFER_BIT);
//  glutSwapBuffers();
//}

int main(int argc, char** argv) {
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//  glutInitWindowSize(1024, 768);
//  glutInitWindowPosition(100, 100);
//  glutCreateWindow("dPhysics Demo");
//  
//  glutDisplayFunc(paint);
//  
//  glClearColor(0.8f, 1.0f, 1.0f, 0.0f);
//  
//  glutMainLoop();
  
  d::vec v1(1, 2, 3), v2(3, 2, 1);
  d::vec v3 = v1 + v2;
  d::print(v3);
  
  return 1;
}
