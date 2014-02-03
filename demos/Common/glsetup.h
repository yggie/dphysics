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

#define ASSERT_NO_GL_ERROR() glAssertNoErr(RE_FILE, __LINE__)

#define EXPECT_NO_GL_ERROR() glExpectNoErr(RE_FILE, __LINE__)

inline void glAssertNoErr(const char* filename, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("[FATAL] %s:%d: OpenGL Error: %s\n", filename, line, gluErrorString(err));
    _RE_KILL_PROGRAM
  }
}

inline void glExpectNoErr(const char* filename, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("[WARN]  %s:%d: OpenGL Error: %s\n", filename, line, gluErrorString(err));
  }
}

#endif
