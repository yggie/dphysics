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

private:
  reVector _color;
  reVector _vect;
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

#endif
