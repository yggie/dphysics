#include "demos/Common/Plane.h"

#include "demos/Common/Canvas.h"

#include "demos/Common/math_ext.h"

using namespace re::demo;

GLuint Plane::_globalVAO = 0;
GLuint Plane::_globalVBO = 0;

Plane::Plane() {
  // do nothing
}

Plane::~Plane() {
  // do nothing
}

void Plane::draw() {
  glBindVertexArray(Plane::_globalVAO);
  glBindBuffer(GL_ARRAY_BUFFER, Plane::_globalVBO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Plane::setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) {
  if (Plane::_globalVAO != 0 ||
      Plane::_globalVBO != 0) {
    printf("[DEMO]  Allocated too many VAO for class Sphere");
  }
  Plane::_globalVAO = vao[0];
  Plane::_globalVBO = vbo[0];
  
  const float vertPos[] = {
     1,  1,  0,
    -1,  1,  0,
     1, -1,  0,
    -1, -1,  0
  };
  
  const float vertNorm[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
  };
  
  // bind buffers
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vao[0]);
  
  // write data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertNorm), nullptr, GL_STATIC_DRAW);
  
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPos), &vertPos[0]);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos), sizeof(vertNorm), &vertNorm[0]);
  
  // vertex positions attribute
  glVertexAttribPointer(canvas.attrs().vertPos(), 3, GL_FLOAT, GL_FALSE, 0, 0);
  // vertex norms attribute
  glVertexAttribPointer(canvas.attrs().vertNorm(), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vertPos));
  
  // ensure required attributes are enabled
  glEnableVertexAttribArray(canvas.attrs().vertPos());
  glEnableVertexAttribArray(canvas.attrs().vertNorm());
}

void Plane::Instance::draw(Canvas& canvas) {
  canvas.push();
  canvas.translate(-center[0], -center[1], -center[2]);
  canvas.scale(breath, width, 1.0);
  canvas.push();
  canvas.loadMatrix(re::demo::facing(normal, side));
  canvas.setMaterial(material);
  canvas.drawUnitPlane();
  canvas.pop();
  canvas.pop();
}

