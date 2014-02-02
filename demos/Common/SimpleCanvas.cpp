#include "demos/Common/SimpleCanvas.h"

#include "demos/Common/Shader.h"

#include <algorithm>
#include <cstdio>

using namespace re::demo;

SimpleCanvas::SimpleCanvas() : Canvas(), _attrs(), _uniforms() {
  _shaders.push_back(new Shader("src/demo/shader.vert", GL_VERTEX_SHADER));
  _shaders.push_back(new Shader("src/demo/shader.frag", GL_FRAGMENT_SHADER));
}

SimpleCanvas::~SimpleCanvas() {
  // do nothing
}

void SimpleCanvas::postInit() {
  _attrs.vertPos = glGetAttribLocation(_programID, "vPos");
  _attrs.vertColor = glGetAttribLocation(_programID, "vColor");
  _attrs.vertNorm = glGetAttribLocation(_programID, "vNorm");

  _uniforms.modelViewMat = glGetUniformLocation(_programID, "mModelView");
  _uniforms.projMat = glGetUniformLocation(_programID, "mProj");
  _uniforms.normMat = glGetUniformLocation(_programID, "mNorm");

  if (_attrs.vertPos == -1) {
    INVALID_ATTRIBUTE_INDEX("vPos")
    throw 0;
  }
  if (_attrs.vertColor == -1) {
    INVALID_ATTRIBUTE_INDEX("vColor")
    throw 0;
  }
  if (_attrs.vertNorm == -1) {
    INVALID_ATTRIBUTE_INDEX("vNorm")
    throw 0;
  }
  if (_uniforms.modelViewMat == -1) {
    INVALID_ATTRIBUTE_INDEX("mModelView")
    throw 0;
  }
  if (_uniforms.projMat == -1) {
    INVALID_ATTRIBUTE_INDEX("mProj")
    throw 0;
  }
  if (_uniforms.normMat == -1) {
    INVALID_ATTRIBUTE_INDEX("mNorm")
    throw 0;
  }
}

void SimpleCanvas::applyModelView() {
  re::mat4 m = viewMat() * modelMat();

  if (_uniforms.modelViewMat != -1) {
    glUniformMatrix4fv(_uniforms.modelViewMat, 1, GL_TRUE, &m[0][0]);
  } else {
    INVALID_ATTRIBUTE_INDEX("mModelView")
  }

  m = re::inverse(re::transpose(m));
  if (_uniforms.normMat != -1) {
    glUniformMatrix4fv(_uniforms.normMat, 1, GL_TRUE, &m[0][0]);
  } else {
    INVALID_ATTRIBUTE_INDEX("mNorm")
  }
}
