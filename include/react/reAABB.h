/**
 * @file
 * Contains the definition of the reAABB class
 */
#ifndef RE_AABB_H
#define RE_AABB_H

#include "react/math.h"

/**
 * An implementation of an axis aligned bounding box
 */

class reAABB {
public:
  reAABB();
  virtual ~reAABB();
  
  // getters
  reFloat width() const;
  reFloat height() const;
  reFloat depth() const;
  reVector& dimens();
  const reVector& dimens() const;
  
  // collision queries
  bool intersects(const reAABB& aabb, const reVector& relPos) const;
  bool isPointInAABB(const reVector& point) const;
  
protected:
  reVector _dimens;
};

inline reAABB::reAABB() : _dimens(0.0, 0.0, 0.0) {
  // do nothing
}

inline reAABB::~reAABB() {
  // do nothing
}

inline reFloat reAABB::width() const {
  return _dimens[0] * 2.0;
}

inline reFloat reAABB::height() const {
  return _dimens[1] * 2.0;
}

inline reFloat reAABB::depth() const {
  return _dimens[2] * 2.0;
}

inline reVector& reAABB::dimens() {
  return _dimens;
}

inline const reVector& reAABB::dimens() const {
  return _dimens;
}

inline bool reAABB::intersects(const reAABB& aabb, const reVector& relPos) const {
  return (reAbs(relPos.x) < _dimens[0] + aabb._dimens[0] + RE_FP_TOLERANCE) &&
         (reAbs(relPos.y) < _dimens[1] + aabb._dimens[1] + RE_FP_TOLERANCE) &&
         (reAbs(relPos.z) < _dimens[2] + aabb._dimens[2] + RE_FP_TOLERANCE);
}

inline bool reAABB::isPointInAABB(const reVector& point) const {
  return (reAbs(point.x) < _dimens[0] + RE_FP_TOLERANCE) &&
         (reAbs(point.y) < _dimens[1] + RE_FP_TOLERANCE) &&
         (reAbs(point.z) < _dimens[2] + RE_FP_TOLERANCE);
}

#endif
