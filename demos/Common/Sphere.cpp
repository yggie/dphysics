#include "demos/Common/Sphere.h"

#include "demos/Common/Canvas.h"
#include "demos/Common/MatrixStack.h"

#include <glm/vec4.hpp>
#include <cmath>

using namespace re::demo;

GLuint Sphere::_globalVAO = 0;
GLuint Sphere::_globalVBO = 0;

const int nSlices = 10; // number of slices
const int nVerts = nSlices * nSlices * 2;

Sphere::Sphere() {
  // do nothing
}

Sphere::~Sphere() {
  // do nothing
}

void Sphere::draw() {
  glBindVertexArray(Sphere::_globalVAO);
  glBindBuffer(GL_ARRAY_BUFFER, Sphere::_globalVBO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVerts);
}

void Sphere::setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) {
  if (Sphere::_globalVAO != 0 ||
      Sphere::_globalVBO != 0) {
    printf("[DEMO]  Allocated too many VAO for class Sphere");
  }
  Sphere::_globalVAO = vao[0];
  Sphere::_globalVBO = vbo[0];
  
  re::vec3 vertPos[nVerts];
  re::vec3 vertNorm[nVerts];
//  glm::vec4 vertColor[nVerts];
  
  int cnt = 0;
  const double radius = 1;
  double s[nSlices+1];
  double c[nSlices+1];
  double r[nSlices+1];
  double z[nSlices+1];
  for (int i = 0; i <= nSlices; i++) {
    double a = 2 * RE_PI * i / (double)nSlices;
    double b = a / 2;
    s[i] = sin(a);
    c[i] = cos(a);
    r[i] = radius * sin(b);
    z[i] = radius * cos(b);
  }
  
  for (int i = 0; i < nSlices; i++) {
    for (int j = 0; j < nSlices; j++) {
      for (int k = 0; k < 2; k++) {
        int ik, jk, ix;
        if (i % 2 == 0) {
          ix = -1;
          ik = i + k;
          jk = j + k;
        } else {
          ix = 1;
          ik = i + 1 - k;
          jk = j + k;
        }
        vertPos[cnt] = re::vec3(r[jk]*s[ik], r[jk]*c[ik], z[jk]*ix);
        vertNorm[cnt] = re::normalize(vertPos[cnt]);
//        vertColor[cnt] = glm::vec4(c[ik], s[ik], 1.0f, 0.9f);
        cnt++;
      }
    }
  }
  
  // bind buffers
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vao[0]);
  
  // write data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertNorm), nullptr, GL_STATIC_DRAW);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertColor) + sizeof(vertNorm), nullptr, GL_STATIC_DRAW);
  
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPos), &vertPos[0][0]);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos), sizeof(vertNorm), &vertNorm[0][0]);
//  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertNorm), sizeof(vertColor), &vertColor[0][0]);
  
  // vertex positions attribute
  glVertexAttribPointer(canvas.attrs().vertPos(), 3, GL_FLOAT, GL_FALSE, 0, 0);
  // vertex norms attribute
  glVertexAttribPointer(canvas.attrs().vertNorm(), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vertPos));
  // vertex colors attribute
//  glVertexAttribPointer(canvas.attrs().vertColor(), 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(vertPos) + sizeof(vertNorm)));
  
  // ensure required attributes are enabled
  glEnableVertexAttribArray(canvas.attrs().vertPos());
  glEnableVertexAttribArray(canvas.attrs().vertNorm());
//  glEnableVertexAttribArray(canvas.attrs().vertColor());
  
  RE_ASSERT_MSG(cnt == nVerts, "Number expected was wrong");
}

void Sphere::Wrapper::draw(Canvas& canvas) {
  canvas.push();
  re::quat q = entity.quat();
  canvas.translate(entity.pos()[0], entity.pos()[1], entity.pos()[2]);
  canvas.scale(shape().radius()*1.05);
  const reFloat ang = acos(q.r);
  if (re::abs(ang) > RE_FP_TOLERANCE) {
    const reFloat s = re::sin(ang);
    canvas.rotate(ang, q.i / s, q.j / s, q.k / s);
  }
  canvas.setMaterial(material);
  canvas.drawUnitSphere();
  canvas.pop();
}

