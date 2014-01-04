#include "ray/parser.h"

#include "ray/RayScene.h"

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdio.h>

int windowWidth, windowHeight;
int imageWidth, imageHeight;

void initFunc() {
  glClearColor(0, 0, 0, 1);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
  glDisable(GL_LOGIC_OP);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
  glPixelTransferi(GL_RED_SCALE, 1);
  glPixelTransferi(GL_RED_BIAS, 0);
  glPixelTransferi(GL_GREEN_SCALE, 1);
  glPixelTransferi(GL_GREEN_BIAS, 0);
  glPixelTransferi(GL_BLUE_SCALE, 1);
  glPixelTransferi(GL_BLUE_BIAS, 0);
  glPixelTransferi(GL_ALPHA_SCALE, 1);
  glPixelTransferi(GL_ALPHA_BIAS, 0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glShadeModel(GL_FLAT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void keyFunc(unsigned char key, int, int) {
  switch (key) {
    case 27:
      glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                    GLUT_ACTION_CONTINUE_EXECUTION);
      glutLeaveMainLoop();
      break;
      
    default:
      printf("Unhandled character input: %c\n", key);
      break;
  }
}

void reshapeFunc(int w, int h) {
  h = (h > 0) ? h : 1;
  windowHeight = h;
  windowWidth = w;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRasterPos3f(-1.0f, 1.0f, -0.5f);
  glPixelZoom(windowWidth/double(imageWidth), -windowHeight/double(imageHeight));
  glOrtho(0, imageWidth, 0, imageHeight, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
}

#include "ray/RayScene.h"

int main(int argc, char** argv) {
//  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//  glutInitWindowSize(640, 480);
//  glutInitWindowPosition(200, 150);
//  glutCreateWindow("Simple Ray Tracing");
//  
//  glutReshapeFunc(reshapeFunc);
//  glutKeyboardFunc(keyFunc);
//  
//  glutDisplayFunc(initFunc);
//  glutMainLoopEvent();
//  glutDisplayFunc(displayFunc);

  createSceneFromFile("res/ray/samples/scene4-specular.test");
  
//  glutMainLoop();
  return 0;
}
