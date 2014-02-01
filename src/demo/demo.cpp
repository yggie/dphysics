#include "react/react.h"
#include "react/debug.h"
#include "demo/App.h"
#include "demo/StaticGfx.h"

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

void mmmprint(const glm::mat4& m) {
  for (int i = 0; i < 4; i++)
    printf(" | %+6.3f, %+6.3f, %+6.3f, %+6.3f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
}

void testDemo(reWorld& world, demo::App& app) {
//  reRigidBody* body;
  reSphere sphere(1.0);
  
  reBuilder build = world.build();
  
  reRigidBody& body = build.RigidBody(sphere).withMass(5.0).at(0, 0, -2).rotatingWith(0.0, 0.00, 0.01);
  app.newPlainSphere(body);
  
//  for (int i = 0; i < 5; i++) {
//    reRigidBody& body = world.newRigidBody(sphere.withRadius(1)).withMass(5.0).at(2.5*i - 3 , 1, -5).facing(re::vec3(0.0, -1.0, 1.0), re::vec3(0.0, 1.0, 0.0)).rotatingWith(0.0, 0.01, 0.0).movingAt(re::vec3(0.01, 0.0, 0.0));
//    app.newPlainSphere(body);
//  }
  
  reRigidBody& A = build.RigidBody(sphere).withMass(2.0).at(-3, -1, -5).movingAt(re::vec3(0.01, 0.0, 0.0));
  app.newPlainSphere(A);
  reRigidBody& B = build.RigidBody(sphere).withMass(2.0).at(3, 3, -5);
  app.newPlainSphere(B);
  
  build.GravAction(A, B);
  build.GravAction(body, B);
  build.GravAction(body, A);
  
  printf("[DEBUG] reIntegrator: %ld:%ld, reTreeBalanceStrategy: %ld:%ld\n", sizeof(reIntegrator), alignof(reIntegrator), sizeof(reTreeBalanceStrategy), alignof(reTreeBalanceStrategy));
  
  const float m = 10.0f;
  const float pos[] = {
    m, -1, m,
    -m, -1, m,
    m, -1, -m,
    -m, -1, -m
  };
  
  const float color[] = {
    0.6f, 0.1f, 0.0f,
    0.6f, 0.1f, 0.0f,
    0.6f, 0.1f, 0.0f,
    0.6f, 0.1f, 0.0f
  };
  
  const float norm[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  };
  
  demo::StaticGfx& floor = app.newStaticGfx();
  floor.newVAO(GL_TRIANGLE_STRIP, 4)
          .withAttrib(app.canvas().attrs().vertPos, sizeof(pos), &pos[0], 3)
          .withAttrib(app.canvas().attrs().vertNorm, sizeof(norm), &norm[0], 3)
          .withAttrib(app.canvas().attrs().vertColor, sizeof(color), &color[0], 3);
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
