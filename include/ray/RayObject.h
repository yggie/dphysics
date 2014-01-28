#ifndef RAY_OBJECT_H
#define RAY_OBJECT_H

#include "react/math.h"

class RayObject {
public:
  RayObject();
  ~RayObject();
  
  RayObject& withDiffuse(float r, float g, float b);
  RayObject& withSpecular(float r, float g, float b);
  RayObject& withEmission(float r, float g, float b);
  RayObject& withShininess(float s);
  
  const re::vec& diffuse() const;
  const re::vec& emission() const;
  const re::vec& specular() const;
  float shininess() const;
  
private:
  re::vec _diffuse;
  re::vec _emission;
  re::vec _specular;
  float _shininess;
};

inline RayObject::RayObject() : _diffuse(), _emission(), _specular(), _shininess(0.0f) {
  // do nothing
}

inline RayObject::~RayObject() {
  // do nothing
}

inline RayObject& RayObject::withDiffuse(float r, float g, float b) {
  _diffuse.set(r, g, b);
  return *this;
}

inline RayObject& RayObject::withSpecular(float r, float g, float b) {
  _specular.set(r, g, b);
  return *this;
}

inline RayObject& RayObject::withEmission(float r, float g, float b) {
  _emission.set(r, g, b);
  return *this;
}

inline RayObject& RayObject::withShininess(float s) {
  _shininess = s;
  return *this;
}

inline const re::vec& RayObject::diffuse() const {
  return _diffuse;
}

inline const re::vec& RayObject::emission() const {
  return _emission;
}

inline const re::vec& RayObject::specular() const {
  return _specular;
}

inline float RayObject::shininess() const {
  return _shininess;
}

#endif
