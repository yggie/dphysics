#include "demo/PlainSphere.h"

#include "demo/MyProgram.h"
#include "demo/App.h"

#include "demo/MatrixStack.h"

#include <cmath>

using namespace demo;

GLint PlainSphere::_globalVAO = -1;
GLint PlainSphere::_globalVBO = -1;

const int nSides = 20; // number of sides
const int nVerts = 2 * (nSides + 1) * (nSides-3);

PlainSphere::PlainSphere(const re::Ent& ent) : GfxObj(ent) {
  // do nothing
}

PlainSphere::~PlainSphere() {
  // do nothing
}

void PlainSphere::gDraw(MatrixStack& stack, const Program& program) {
  stack.push();
  stack.top().scale(shape().radius());
  stack.top().translate(_ent->pos()[0], _ent->pos()[1], _ent->pos()[2]);
  program.setModelMat(stack.getMatrix());
  glBindVertexArray(_globalVAO);
  glBindBuffer(GL_ARRAY_BUFFER, _globalVBO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVerts);
  stack.pop();
}

void PlainSphere::gSetup(GLint vao, GLint vbo, const MyProgram& program) {
  if (PlainSphere::_globalVAO != -1 || PlainSphere::_globalVBO != -1) {
    printf("[DEMO]  Allocated too many VAO for class PlainSphere");
  }
  PlainSphere::_globalVAO = vao;
  PlainSphere::_globalVBO = vbo;
  
  float vertPos[3 * nVerts];
  float vertColor[4 * nVerts];
  
  int idx = 0;
  const double radius = 1;
  for (int i = 0; i < nSides - 3; i++) {
    double a1 = PI * (i + 1.0) / (double)(nSides - 1);
    double a2 = PI * (i + 2.0) / (double)(nSides - 1);
    
    float r1 = radius * sin(a1);
    float r2 = radius * sin(a2);
    
    float z1 = radius * cos(a1);
    float z2 = radius * cos(a2);
    
    for (int j = 0; j <= nSides; j++) {
      double b = 2 * PI * (nSides - j) / (double)nSides;
      double c = cos(b);
      double s = sin(b);
      
      vertPos[3*idx + 0] = r1 * s;
      vertPos[3*idx + 1] = r1 * c;
      vertPos[3*idx + 2] = z1;
      vertColor[4*idx + 0] = c;
      vertColor[4*idx + 1] = s;
      vertColor[4*idx + 2] = 1.0f;
      vertColor[4*idx + 3] = 0.9f;
      idx++;
      vertPos[3*idx + 0] = r2 * s;
      vertPos[3*idx + 1] = r2 * c;
      vertPos[3*idx + 2] = z2;
      vertColor[4*idx + 0] = c;
      vertColor[4*idx + 1] = s;
      vertColor[4*idx + 2] = 1.0f;
      vertColor[4*idx + 3] = 0.9f;
      idx++;
    }
  }
  
  printf("Expected: %d got %d\n", nVerts, idx);
  
  // bind buffers
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  // write data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertColor),
               nullptr, GL_STATIC_DRAW);
  
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPos), vertPos);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos), sizeof(vertColor), vertColor);
  
  // program pointers
  glVertexAttribPointer(program.vertPosAddr(), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(program.vertPosAddr());
  
  glVertexAttribPointer(program.vertColorAddr(), 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vertPos));
  glEnableVertexAttribArray(program.vertColorAddr());
}

