#include <GL/freeglut.h>

#include "react/react.h"
#include "react/utils.h"

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

  re::Sphere sphere;
  
  re::World world;
  re::Factory factory = world.factory().as(re::Entity::RIGID);
  
  for (int i = 0; i < 5; i++) {
    factory.make();
  }
  
  printf("Hurray no problems!\n");
  
  return 0;
}
