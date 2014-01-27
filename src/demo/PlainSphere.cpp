#include "demo/PlainSphere.h"

#include "react/common.h"
#include "demo/Canvas.h"
#include "demo/App.h"
#include "demo/MatrixStack.h"

#include <glm/vec4.hpp>
#include <cmath>

using namespace demo;

GLuint PlainSphere::_globalVAO = 0;
GLuint PlainSphere::_globalVBO = 0;

const int nSlices = 10; // number of slices
const int nVerts = nSlices * nSlices * 2;

PlainSphere::PlainSphere(const reEnt& ent) : _ent(ent) {
  shape(); // ensure the shape is correct
}

PlainSphere::~PlainSphere() {
  // do nothing
}

void PlainSphere::draw(Canvas& canvas) {
//  canvas.modelMat();
  canvas.push();
  re::quat q = _ent.quat();
  canvas.translate(_ent.pos()[0], _ent.pos()[1], _ent.pos()[2]);
  canvas.scale(shape().radius());
  const reFloat ang = acos(q.r);
  if (reAbs(ang) > RE_FP_TOLERANCE) {
    const reFloat s = reSin(ang);
    printf("%.2f, %.2f (%.2f, %.2f, %.2f) \n", ang, s, q.i, q.j, q.k);
    canvas.rotate(2 * ang * 180.0 / RE_PI, q.i / s, q.j / s, q.k / s);
  }
  canvas.applyModelView();
  glBindVertexArray(PlainSphere::_globalVAO);
  glBindBuffer(GL_ARRAY_BUFFER, PlainSphere::_globalVBO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVerts);
  canvas.pop();
}

void PlainSphere::setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) {
  if (PlainSphere::_globalVAO != 0 ||
      PlainSphere::_globalVBO != 0) {
    printf("[DEMO]  Allocated too many VAO for class PlainSphere");
  }
  PlainSphere::_globalVAO = vao[0];
  PlainSphere::_globalVBO = vbo[0];
  
  glm::vec3 vertPos[nVerts];
  glm::vec3 vertNorm[nVerts];
  glm::vec4 vertColor[nVerts];
  
  int cnt = 0;
  const double radius = 1;
  double s[nSlices+1];
  double c[nSlices+1];
  double r[nSlices+1];
  double z[nSlices+1];
  for (int i = 0; i <= nSlices; i++) {
    double a = 2 * PI * i / (double)nSlices;
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
        vertPos[cnt] = glm::vec3(r[jk]*s[ik], r[jk]*c[ik], z[jk]*ix);
        vertNorm[cnt] = glm::normalize(vertPos[i]);
        vertColor[cnt++] = glm::vec4(c[ik], s[ik], 1.0f, 0.9f);
      }
    }
  }
  
  // bind buffers
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vao[0]);
  
  // write data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertColor) + sizeof(vertNorm), nullptr, GL_STATIC_DRAW);
  
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertPos), &vertPos[0][0]);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos), sizeof(vertNorm), &vertNorm[0][0]);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertPos) + sizeof(vertNorm), sizeof(vertColor), &vertColor[0][0]);
  
  // vertex positions attribute
  glVertexAttribPointer(canvas.attrs().vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // vertex norms attribute
  glVertexAttribPointer(canvas.attrs().vertNorm, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vertPos));
  // vertex colors attribute
  glVertexAttribPointer(canvas.attrs().vertColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(vertPos) + sizeof(vertNorm)));
  
  // ensure required attributes are enabled
  glEnableVertexAttribArray(canvas.attrs().vertPos);
  glEnableVertexAttribArray(canvas.attrs().vertNorm);
  glEnableVertexAttribArray(canvas.attrs().vertColor);
  
  printf("[DEMO]  Expected: %d got %d\n", nVerts, cnt);
  RE_ASSERT_MSG(cnt == nVerts, "Number expected was wrong");
}

