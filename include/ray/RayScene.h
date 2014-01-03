#ifndef RAY_SCENE_H
#define RAY_SCENE_H

#include "ray/RayImage.h"
#include "react/reWorld.h"
#include "react/reRigidBody.h"
#include "react/reSphere.h"

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class RayScene {
public:
  RayScene();
  ~RayScene();
  
  void setOutputFile(const char* filename);
  void setSize(unsigned int w, unsigned int h);
  void setMaxBounces(unsigned int maxBounces);
  
  void setFOVY(float fovy);
  void setViewMat(const glm::mat4& view);
  void regenerateProjMat();
  
  void addSphere(float x, float y, float z, float r);
  void addTriangle(const reVector& a, const reVector& b, const reVector& c);
  
private:
  RayImage _image;
  unsigned int _width;
  unsigned int _height;
  unsigned int _maxBounces;
  std::string _outputFile;
  
  float _fovy;
  glm::mat4 _projMat;
  glm::mat4 _viewMat;
  
  reWorld _world;
};

inline RayScene::RayScene() : _image(), _width(0), _height(0),
_maxBounces(5), _outputFile(), _fovy(120.0f), _projMat(1.0f),
_viewMat(1.0f), _world() {
  // do nothing
}

inline RayScene::~RayScene() {
  // do nothing
}

inline void RayScene::setOutputFile(const char* filename) {
  _outputFile = filename;
}

inline void RayScene::setSize(unsigned int w, unsigned int h) {
  _width = w;
  _height = h;
  regenerateProjMat();
}

inline void RayScene::setMaxBounces(unsigned int maxBounces) {
  _maxBounces = maxBounces;
}

inline void RayScene::setFOVY(float fovy) {
  _fovy = fovy;
  regenerateProjMat();
}

inline void RayScene::setViewMat(const glm::mat4& view) {
  _viewMat = view;
}

inline void RayScene::regenerateProjMat() {
  float tanFOVY = glm::tan(_fovy);
  float ratio = _width / double(_height);
  _projMat = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, 1.0f + 1.0f/tanFOVY);
}

inline void RayScene::addSphere(float x, float y, float z, float r) {
  _world.newRigidBody().withShape(reSphere(r)).at(x, y, z);
}

inline void RayScene::addSphere(const reVector& a, const reVector& b, const reVector& c) {
  _world.newRigidBody().withShape(reSphere(r)).at(x, y, z);
}

#endif
