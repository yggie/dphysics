#include <GL/freeglut.h>
#include "react/vector.h"
#include "react/matrix.h"
#include "react/debug.h"

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
  
  re::vec v1(1, 2, 3), v2(3, 2, 1);
  re::vec v3 = v1 + v2;
  v3 += 3.0;
  v3 /= 7;
  v3 *= 4.0;
  v3 -= 3.0;
  v3 = v3 - re::vec(1.0, 0.0, 0.0);
  re::print(v3);
  
  v3.set(3.0, 3.0, 3.0);
  re::vec v4 = v3.cross(re::vec(-1, -1, 1));
  re::print(v4);
  printf("%f\n", v3.dot(v1));
  
  v4.set(4, 5, 6);
  re::print(v4);
  
  re::mat m;
  re::print(m);
  
  v3 = m * v4;
  re::print(v3);
  
  m *= 3;
  v3 = m * v4;
  re::print(v3);
  
  return 0;
}
