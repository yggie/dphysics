#ifndef DEMO_GLSETUP_H
#define DEMO_GLSETUP_H

#include "react/common.h"

// enable the use of shaders
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <cstdio>

const int LOG_BUFFER_LENGTH = 255;

#define CHECK_GL_ERR glErr(__FILE__, __LINE__)

inline bool glErr(const char* filename, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("[DEMO] %s:%d: OpenGL error ~ %s\n", filename, line, gluErrorString(err));
    return true;
  }
  return false;
}

#endif
