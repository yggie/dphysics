#ifndef RAY_LIGHTSOURCE_H
#define RAY_LIGHTSOURCE_H

#include "react/math.h"

/**
 * @brief The RayLightSource class
 */

class RayLightSource {
public:
  RayLightSource();
  ~RayLightSource();

  RayLightSource& withColor(const re::vec3& color);
  RayLightSource& withVect(const re::vec3& vect);
  RayLightSource& asDirectional(bool directional);
  
  re::vec3& transformedVect();
  const re::vec3& transformedVect() const;
  const re::vec3& vect() const;
  const re::vec3& color() const;
  bool isDirectional() const;

private:
  re::vec3 _color;
  re::vec3 _vect;
  re::vec3 _transformedVect;
  bool _isSpot;
};

inline RayLightSource::RayLightSource() : _color(), _vect(), _isSpot(true) {
  // do nothing
}

inline RayLightSource::~RayLightSource() {
  // do nothing
}

inline RayLightSource& RayLightSource::withColor(const re::vec3& color) {
  _color = color;
  return *this;
}

inline RayLightSource& RayLightSource::withVect(const re::vec3& vect) {
  _vect = vect;
  return *this;
}

inline RayLightSource& RayLightSource::asDirectional(bool directional) {
  _isSpot = !directional;
  return *this;
}

inline re::vec3& RayLightSource::transformedVect() {
  return _transformedVect;
}

inline const re::vec3& RayLightSource::transformedVect() const {
  return _transformedVect;
}

inline const re::vec3& RayLightSource::vect() const {
  return _vect;
}

inline const re::vec3& RayLightSource::color() const {
  return _color;
}

inline bool RayLightSource::isDirectional() const {
  return !_isSpot;
}

#endif
