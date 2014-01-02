#include "demo/MyProgram.h"

#include <algorithm>

#include <cstdio>

using namespace demo;

MyProgram::MyProgram() : Program(), _vertPosAddr(-1),
_vertColorAddr(-1), _modelMatAddr(-1), _viewMatAddr(-1),
_projMatAddr(-1) {
  _shaders.push_back(new Shader("src/demo/shader.vert", GL_VERTEX_SHADER));
  _shaders.push_back(new Shader("src/demo/shader.frag", GL_FRAGMENT_SHADER));
}

MyProgram::~MyProgram() {
  // do nothing
}

void MyProgram::_init() {
  _vertPosAddr = glGetAttribLocation(_id, "vPos");
  _vertColorAddr = glGetAttribLocation(_id, "vColor");
  
  _modelMatAddr = glGetUniformLocation(_id, "mModel");
  _viewMatAddr = glGetUniformLocation(_id, "mView");
  _projMatAddr = glGetUniformLocation(_id, "mProj");
  
  mat4 ident;
  ident.setIdentity();
  
  if (_vertPosAddr == -1) {
    printf("[DEMO]  Vertex shader variable \"vPos\" not found\n");
    throw 0;
  }
  if (_vertColorAddr == -1) {
    printf("[DEMO]  Vertex shader variable \"vColor\" not found\n");
    throw 0;
  }
  if (_modelMatAddr == -1) {
    printf("[DEMO]  Vertex shader uniform \"mModel\" not found\n");
    throw 0;
  }
  if (_viewMatAddr == -1) {
    printf("[DEMO]  Vertex shader uniform \"mView\" not found\n");
    throw 0;
  }
  if (_projMatAddr == -1) {
    printf("[DEMO]  Vertex shader uniform \"mProj\" not found\n");
    throw 0;
  }
}

