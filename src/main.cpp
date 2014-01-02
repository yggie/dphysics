#include "react/react.h"
#include "react/utils.h"
#include "demo/App.h"

#include <cstdio>

demo::App app;

// proxy functions, because glut won't accept otherwise
void keyboardFunc(unsigned char key, int x, int y) {
  app.keyEvent(key, x, y);
}

void specialKeys(int key, int x, int y) {
  app.specialKeyEvent(key, x, y);
}

void paint(void) {
  app.gPaint();
}

void reshape(int w, int h) {
  app.gResizeScreen(w, h);
}

void init(void) {
  app.gInit();
}

void testDemo(re::World& world, demo::App& app) {
//  re::RigidBody* body;
  re::Sphere sphere(1.0);
  
  re::RigidBody& body = world.newRigidBody().withShape(sphere).withMass(5.0).at(0, 0, -2);
  app.newPlainSphere(body);
  
//  for (int i = 0; i < 10; i++) {
//    body = (re::RigidBody*)&world.newRigidBody().withShape(sphere.withRadius(i + 1)).withMass(5.0).at(i-5 , i-5, i-5);
//    app.newPlainSphere(*body);
//  }
  
//  re::print(body->inertia());
  re::print(body.inertia());
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(1024, 768);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("react Physics Engine Demo");
  
  glutKeyboardFunc(keyboardFunc);
  glutReshapeFunc(reshape);
  glutSpecialFunc(specialKeys);
  
  app.addDemo(testDemo);
  
  // hax to get the init to work
  glutDisplayFunc(init);
  glutMainLoopEvent();
  glutDisplayFunc(paint);
  glutMainLoop();
  
  printf("Hurray no problems!\n");
  
  return 0;
}
