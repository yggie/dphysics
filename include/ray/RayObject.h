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
  
  const reVector& diffuse() const;
  const reVector& emission() const;
  const reVector& specular() const;
  float shininess() const;
  
private:
  reVector _diffuse;
  reVector _emission;
  reVector _specular;
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

inline const reVector& RayObject::diffuse() const {
  return _diffuse;
}

inline const reVector& RayObject::emission() const {
  return _emission;
}

inline const reVector& RayObject::specular() const {
  return _specular;
}

inline float RayObject::shininess() const {
  return _shininess;
}

#endif
