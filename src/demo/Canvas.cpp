#include "demo/Canvas.h"

#include "demo/glsetup.h"
#include "demo/Shader.h"

#include <algorithm>
#include <cstdio>

using namespace demo;

Canvas::Canvas() : _programID(0), _shaders(), _projMat(1.0f),
_viewMat(1.0f), _modelMatStack() {
}

Canvas::~Canvas() {
  release();
}

void Canvas::init() {
  if (_programID != 0) {
    printf("[DEMO]  Shader program already created\n");
    return;
  }
  
  for_each(_shaders.begin(), _shaders.end(), [](Shader* shader) {
    if (!shader->compile()) {
      throw 0;
    }
  });
  
  _programID = glCreateProgram();
  for_each(_shaders.begin(), _shaders.end(), [&](Shader* shader) {
    glAttachShader(_programID, shader->id());
  });
  
  glLinkProgram(_programID);
  if (isFalse(GL_LINK_STATUS)) {
    printLog();
    throw 0;
  }
  
  postInit();
  
  glUseProgram(_programID);
}
  
void Canvas::use() {
  if (_programID != 0) {
    glUseProgram(_programID);
  } else {
    PROGRAM_ID_NOT_SET
  }
}

/**
 * Releases any resources used by the Canvas
 */

void Canvas::release() {
  GLint currProgID;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currProgID);
  if ((GLuint)currProgID == _programID) {
    glUseProgram(0);
  }
  
  unsigned int ids[] = { 0, 0 };
  int i = 0;
  for_each(_shaders.begin(), _shaders.end(), [&](Shader* shader) {
    if (_programID != 0) {
      glDetachShader(_programID, shader->id());
      ids[i++] = shader->id();
    }
    delete shader;
  });
  _shaders.clear();
  
  if (_programID != 0) {
    glDeleteProgram(_programID);
  }
  
  _programID = 0;
}

bool Canvas::isFalse(GLenum option) const {
  GLint status;
  glGetProgramiv(_programID, option, &status);
  return (status == GL_FALSE);
}

void Canvas::printLog() {
  if (_programID != 0) {
    GLint len;
    char buffer[LOG_BUFFER_LENGTH];
    glGetProgramInfoLog(_programID, LOG_BUFFER_LENGTH, &len, &buffer[0]);
    printf("%s\n", &buffer[0]);
  } else {
    PROGRAM_ID_NOT_SET
  }
}

