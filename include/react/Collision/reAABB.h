/**
 * @file
 * Contains the definition of the reAABB class
 */
#ifndef RE_AABB_H
#define RE_AABB_H

#include "react/math.h"

/**
 * @ingroup collision
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
  re::vec3& dimens();
  const re::vec3& dimens() const;
  
  // collision queries
  bool intersects(const reAABB& aabb, const re::vec3& relPos) const;
  bool containsPoint(const re::vec3& point) const;
  
protected:
  re::vec3 _dimens;
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

inline re::vec3& reAABB::dimens() {
  return _dimens;
}

inline const re::vec3& reAABB::dimens() const {
  return _dimens;
}

inline bool reAABB::intersects(const reAABB& aabb, const re::vec3& relPos) const {
  return (re::abs(relPos.x) < _dimens[0] + aabb._dimens[0] + RE_FP_TOLERANCE) &&
         (re::abs(relPos.y) < _dimens[1] + aabb._dimens[1] + RE_FP_TOLERANCE) &&
         (re::abs(relPos.z) < _dimens[2] + aabb._dimens[2] + RE_FP_TOLERANCE);
}

inline bool reAABB::containsPoint(const re::vec3& point) const {
  return (re::abs(point.x) < _dimens[0] + RE_FP_TOLERANCE) &&
         (re::abs(point.y) < _dimens[1] + RE_FP_TOLERANCE) &&
         (re::abs(point.z) < _dimens[2] + RE_FP_TOLERANCE);
}

#endif
