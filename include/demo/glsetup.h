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

inline bool checkOpenGLError() {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    RE_WARN("OpenGL error: %s", gluErrorString(err));
    return true;
  }
  return false;
}

#endif
