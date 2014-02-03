#include "demos/Common/SimpleCanvas.h"

#include "demos/Common/Shader.h"

#include <algorithm>
#include <cstdio>

using namespace re::demo;

SimpleCanvas::SimpleCanvas() : Canvas() {
  _shaders.push_back(new Shader("demos/Shaders/shader.vert", GL_VERTEX_SHADER));
  _shaders.push_back(new Shader("demos/Shaders/shader.frag", GL_FRAGMENT_SHADER));
}

SimpleCanvas::~SimpleCanvas() {
  // do nothing
}

void SimpleCanvas::postInit() {
  // do nothing
}

void SimpleCanvas::applyModelView() {
  re::mat4 m = viewMat() * modelMat();
  glUniformMatrix4fv(_uniforms.modelViewMat(), 1, GL_TRUE, &m[0][0]);

  m = re::inverse(re::transpose(m));
  glUniformMatrix4fv(_uniforms.normMat(), 1, GL_TRUE, &m[0][0]);
}
