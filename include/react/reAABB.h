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
  
  reFloat width() const;
  reFloat height() const;
  reFloat depth() const;
  const reFloat& halfWidth() const;
  const reFloat& halfHeight() const;
  const reFloat& halfDepth() const;
  void setHalfWidth(reFloat halfWidth);
  void setHalfHeight(reFloat halfHeight);
  void setHalfDepth(reFloat halfDepth);
  void set(reFloat halfWidth, reFloat halfHeight, reFloat halfDepth);
  bool intersects(const reAABB& aabb, const reVector& relPos) const;
  bool isPointInAABB(const reVector& point) const;
  
protected:
  reFloat _halfWidth;
  reFloat _halfHeight;
  reFloat _halfDepth;
};

inline reAABB::reAABB() : _halfWidth(0.0), _halfHeight(0.0), _halfDepth(0.0) {
  // do nothing
}

inline reAABB::~reAABB() {
  // do nothing
}

inline reFloat reAABB::width() const {
  return _halfWidth * 2.0;
}

inline reFloat reAABB::height() const {
  return _halfHeight * 2.0;
}

inline reFloat reAABB::depth() const {
  return _halfDepth * 2.0;
}

inline const reFloat& reAABB::halfWidth() const {
  return _halfWidth;
}

inline const reFloat& reAABB::halfHeight() const {
  return _halfHeight;
}

inline const reFloat& reAABB::halfDepth() const {
  return _halfDepth;
}

inline void reAABB::setHalfWidth(reFloat halfWidth) {
  _halfWidth = halfWidth;
}

inline void reAABB::setHalfHeight(reFloat halfHeight) {
  _halfHeight = halfHeight;
}

inline void reAABB::setHalfDepth(reFloat halfDepth) {
  _halfDepth = halfDepth;
}

inline void reAABB::set(reFloat halfWidth, reFloat halfHeight, reFloat halfDepth) {
  _halfWidth = halfWidth;
  _halfHeight = halfHeight;
  _halfDepth = halfDepth;
}

inline bool reAABB::intersects(const reAABB& aabb, const reVector& relPos) const {
  return (reAbs(relPos.x) < _halfWidth + aabb._halfWidth + RE_FP_TOLERANCE) &&
         (reAbs(relPos.y) < _halfHeight + aabb._halfHeight + RE_FP_TOLERANCE) &&
         (reAbs(relPos.z) < _halfDepth + aabb._halfDepth + RE_FP_TOLERANCE);
}

inline bool reAABB::isPointInAABB(const reVector& point) const {
  return (reAbs(point.x) < _halfWidth + RE_FP_TOLERANCE) &&
         (reAbs(point.y) < _halfHeight + RE_FP_TOLERANCE) &&
         (reAbs(point.z) < _halfDepth + RE_FP_TOLERANCE);
}

#endif
