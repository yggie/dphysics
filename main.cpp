#include <GL/freeglut.h>
#include "vector.h"
#include "matrix.h"
#include "debug.h"

//static void paint() {
//  glClear(GL_COLOR_BUFFER_BIT);
//  glutSwapBuffers();
//}

int main(int, char**) {
//int main(int argc, char** argv) {
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
  v3 += 3.0;
  v3 /= 7;
  v3 *= 4.0;
  v3 -= 3.0;
  v3 = v3 - d::vec(1.0, 0.0, 0.0);
  d::print(v3);
  
  v3.set(3.0, 3.0, 3.0);
  d::vec v4 = v3.cross(d::vec(-1, -1, 1));
  d::print(v4);
  printf("%f\n", v3.dot(v1));
  
  v4.setZero();
  d::print(v4);
  
  d::mat4 m;
  d::print(m);
  
  return 0;
}
