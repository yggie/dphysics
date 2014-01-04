#ifndef RAY_SCENE_H
#define RAY_SCENE_H

#include "ray/RayImage.h"
#include "ray/RayLightSource.h"
#include "react/reWorld.h"
#include "react/math.h"
#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class RayObject;

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

  void setAmbient(const reVector& ambient);
  void setAttenuation(const reVector& attenuation);
  
  RayObject& addSphere(float x, float y, float z, float r, const glm::mat4& modelMat);
  RayObject& addTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::mat4& modelMat);
  RayLightSource& addSpotLightSource(const reVector& color, const reVector& pos);
  RayLightSource& addDirectionalLightSource(const reVector& color, const reVector& dir);
  
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
  reVector _ambient;
  reVector _attenuation;
  std::vector<RayLightSource*> _lights;
};

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

inline void RayScene::setAmbient(const reVector& ambient) {
  _ambient = ambient;
}

inline void RayScene::setAttenuation(const reVector& attenuation) {
  _attenuation = attenuation;
}

#endif
