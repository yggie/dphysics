#ifndef DEMO_GLSETUP_H
#define DEMO_GLSETUP_H

// enable the use of shaders
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>

const int LOG_BUFFER_LENGTH = 255;

#define checkOpenGLError()    printOglErr(__FILE__, __LINE__)

inline bool printOglErr(const char* file, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("[DEMO]  %s:%d: OpenGL error: %s\n", file, line, gluErrorString(err));
    return true;
  }
  return false;
}

#endif
