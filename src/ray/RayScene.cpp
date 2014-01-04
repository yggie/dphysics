#include "ray/RayScene.h"

#include "react/reRigidBody.h"
#include "react/reSphere.h"
#include "react/reTriangle.h"
#include "react/reDistortedShape.h"
#include "ray/RayObject.h"

#define GLM_FORCE_RADIANS
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

RayScene::RayScene() : _image(), _width(0), _height(0),
_maxBounces(5), _outputFile(), _fovy(120.0f), _projMat(1.0f),
_viewMat(1.0f), _world() {
  // do nothing
}

RayScene::~RayScene() {
  // do nothing
}

void RayScene::setViewMat(const glm::mat4& view) {
  _viewMat = view;
}

void RayScene::regenerateProjMat() {
  float tanFOVY = glm::tan(_fovy);
  float ratio = _width / double(_height);
  _projMat = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, 1.0f + 1.0f/tanFOVY);
}

RayObject& RayScene::addSphere(float x, float y, float z, float r, const glm::mat4& modelMat) {
  reTMatrix tm;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      tm[i][j] = modelMat[i][j];
    }
  }
  reEnt& ent = _world.newRigidBody().withShape(
    reDistortedShape(reSphere(r)).withDistortion(tm)
  ).at(x, y, z);
  RayObject* obj = new RayObject();
  ent.userdata = obj;
  return *obj;
}

RayObject& RayScene::addTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::mat4& modelMat) {
  glm::vec4 a2 = modelMat * glm::vec4(a, 1.0f);
  glm::vec4 b2 = modelMat * glm::vec4(b, 1.0f);
  glm::vec4 c2 = modelMat * glm::vec4(c, 1.0f);
  
  reVector aa, bb, cc;
  for (int i = 0; i < 3; i++) {
    aa[i] = a2[i];
    bb[i] = b2[i];
    cc[i] = c2[i];
  }
  
  reEnt& ent = _world.newRigidBody().withShape(reTriangle(aa, bb, cc)).at(0, 0, 0);
  RayObject* obj = new RayObject();
  ent.userdata = obj;
  return *obj;
}
