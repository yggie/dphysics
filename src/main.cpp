#include "react/react.h"
#include "react/utils.h"
#include "demo/App.h"

#include <cstdio>

demo::App app;

void keyboardFunc(unsigned char key, int x, int y) {
  app.keyEvent(key, x, y);
}

void paint(void) {
  app.gPaint();
}

void reshape(int w, int h) {
  app.gResizeScreen(w, h);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(1024, 768);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("react Physics Engine Demo");
  
  glutDisplayFunc(paint);
  glutKeyboardFunc(keyboardFunc);
  glutReshapeFunc(reshape);
  
//  glClearColor(0.8f, 1.0f, 1.0f, 0.0f);

  re::RigidBody* body;
  re::Sphere sphere(1.0);
  
  re::World world;
  
  for (int i = 0; i < 5; i++) {
    body = (re::RigidBody*)&world.newRigidBody().withShape(sphere.withRadius(i + 1)).withMass(5.0).at(1, i, 1);
  }
  
  re::print(body->inertia());
  
  glutMainLoop();
  
  printf("Hurray no problems!\n");
  
  return 0;
}
