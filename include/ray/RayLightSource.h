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

  RayLightSource& withColor(const re::vec& color);
  RayLightSource& withVect(const re::vec& vect);
  RayLightSource& asDirectional(bool directional);
  
  re::vec& transformedVect();
  const re::vec& transformedVect() const;
  const re::vec& vect() const;
  const re::vec& color() const;
  bool isDirectional() const;

private:
  re::vec _color;
  re::vec _vect;
  re::vec _transformedVect;
  bool _isSpot;
};

inline RayLightSource::RayLightSource() : _color(), _vect(), _isSpot(true) {
  // do nothing
}

inline RayLightSource::~RayLightSource() {
  // do nothing
}

inline RayLightSource& RayLightSource::withColor(const re::vec& color) {
  _color = color;
  return *this;
}

inline RayLightSource& RayLightSource::withVect(const re::vec& vect) {
  _vect = vect;
  return *this;
}

inline RayLightSource& RayLightSource::asDirectional(bool directional) {
  _isSpot = !directional;
  return *this;
}

inline re::vec& RayLightSource::transformedVect() {
  return _transformedVect;
}

inline const re::vec& RayLightSource::transformedVect() const {
  return _transformedVect;
}

inline const re::vec& RayLightSource::vect() const {
  return _vect;
}

inline const re::vec& RayLightSource::color() const {
  return _color;
}

inline bool RayLightSource::isDirectional() const {
  return !_isSpot;
}

#endif
