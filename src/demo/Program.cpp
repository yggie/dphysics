#include "demo/Program.h"

#include "demo/Shader.h"

#include <algorithm>

using namespace demo;

Program::Program() : _id(0), _shaders() {
  // do nothing
}

Program::~Program() {
  del();
}

void Program::init() {
  if (_id != 0) {
    printf("[DEMO]  Program already created\n");
    return;
  }
  
  std::vector<Shader*>::iterator it;
  std::vector<Shader*>::iterator itEnd = _shaders.end();
  
  for (it = _shaders.begin(); it != itEnd; it++) {
    if (!(*it)->compile()) {
      // do something really bad
      throw 0;
    }
  }
  
  _id = glCreateProgram();
  for (it = _shaders.begin(); it != itEnd; it++) {
    glAttachShader(_id, (*it)->id());
  }
  
  glLinkProgram(_id);
  if (isFalse(GL_LINK_STATUS)) {
    printLog();
    throw 0;
  }
  
  _init();
  
  glUseProgram(_id);
}

void Program::del() {
  GLuint currentProgramID;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
  if (currentProgramID == _id) {
    glUseProgram(0);
  }
  
  unsigned int ids[] = { 0, 0 };
  int i = 0;
  for_each(_shaders.begin(), _shaders.end(), [&](Shader* shader) {
    if (_id != 0) {
      glDetachShader(_id, shader->id());
      ids[i++] = shader->id();
    }
    delete shader;
  });
  _shaders.clear();
  
  if (_id != 0) {
    glDeleteProgram(_id);
  }
  
  _id = 0;
}

