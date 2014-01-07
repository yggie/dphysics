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

  RayLightSource& withColor(const reVector& color);
  RayLightSource& withVect(const reVector& vect);
  RayLightSource& asDirectional(bool directional);
  
  reVector& transformedVect();
  const reVector& transformedVect() const;
  const reVector& vect() const;
  const reVector& color() const;
  bool isDirectional() const;

private:
  reVector _color;
  reVector _vect;
  reVector _transformedVect;
  bool _isSpot;
};

inline RayLightSource::RayLightSource() : _color(), _vect(), _isSpot(true) {
  // do nothing
}

inline RayLightSource::~RayLightSource() {
  // do nothing
}

inline RayLightSource& RayLightSource::withColor(const reVector& color) {
  _color = color;
  return *this;
}

inline RayLightSource& RayLightSource::withVect(const reVector& vect) {
  _vect = vect;
  return *this;
}

inline RayLightSource& RayLightSource::asDirectional(bool directional) {
  _isSpot = !directional;
  return *this;
}

inline reVector& RayLightSource::transformedVect() {
  return _transformedVect;
}

inline const reVector& RayLightSource::transformedVect() const {
  return _transformedVect;
}

inline const reVector& RayLightSource::vect() const {
  return _vect;
}

inline const reVector& RayLightSource::color() const {
  return _color;
}

inline bool RayLightSource::isDirectional() const {
  return !_isSpot;
}

#endif
