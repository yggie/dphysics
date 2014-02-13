/**
 * @file
 * Contains the definition of the reProxyShape class
 */
#ifndef RE_TRANSFORMEDSHAPE_H
#define RE_TRANSFORMEDSHAPE_H

#include "react/Collision/Shapes/reShape.h"

/**
 * @ingroup shapes
 * A special wrapper class around an arbitrary shape which allows the
 * user to apply additional transformations on the shape, such as scaling
 * or rotating
 */

class reProxyShape : public reShape {
public:
  reProxyShape(reShape* shape);
  reProxyShape(reShape* shape, const reTransform& transform);
  reProxyShape(const reProxyShape&) = delete;
  ~reProxyShape();
  
  reProxyShape& operator=(const reProxyShape&) = delete;
  
  reShape* shape();
  const reShape* shape() const;
  void setShape(const reShape& base);
  reProxyShape& withShape(const reShape& base);
  
  const reTransform& transform() const;
  void setTransform(const reTransform& transform);
  reProxyShape& withTransform(const reTransform& transform);
  
  // shape representation
  reShape::Type type() const override;
  reUInt numVerts() const override;
  const re::vec3 vert(reUInt i) const override;
  reFloat shell() const override;
  const re::vec3 center() const override;
  
  // physical metrics
  reFloat volume() const override;
  const re::mat3 computeInertia() const override;
  
  // utility methods
  const re::vec3 randomPoint() const override;
  
  // collision queries
  bool containsPoint(const re::vec3& point) const override;
  
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;
  
  re::Plane::Location locationInPlane(const re::Plane& plane) const override;
  
private:
  reTransform _transform;
  reShape* const _shape;
};

inline void reProxyShape::setTransform(const reTransform& transform) {
  _transform = transform;
}

inline reProxyShape& reProxyShape::withTransform(const reTransform& transform) {
  setTransform(transform);
  return *this;
}

inline reShape* reProxyShape::shape() {
  return _shape;
}

inline const reShape* reProxyShape::shape() const {
  return _shape;
}

inline const reTransform& reProxyShape::transform() const {
  return _transform;
}

inline reShape::Type reProxyShape::type() const {
  return reShape::PROXY;
}

inline reUInt reProxyShape::numVerts() const {
  if (_shape != nullptr) {
    return _shape->numVerts();
  } else {
    return 0;
  }
}

inline const re::vec3 reProxyShape::vert(reUInt i) const {
  if (_shape != nullptr) {
    return _shape->vert(i);
  } else {
    return re::vec(0.0, 0.0, 0.0);
  }
}

inline reFloat reProxyShape::shell() const {
  if (_shape != nullptr) {
    return _shape->shell();
  } else {
    return reShape::shell();
  }
}

inline const re::vec3 reProxyShape::center() const {
  if (_shape != nullptr) {
    return _shape->center() + _transform.v;
  } else {
    return _transform.v;
  }
}

inline reFloat reProxyShape::volume() const {
//  RE_NOT_IMPLEMENTED
  if (_shape != nullptr) {
    return _shape->volume();
  } else {
    return 0.0;
  }
}

inline const re::mat3 reProxyShape::computeInertia() const {
//  RE_NOT_IMPLEMENTED
  return re::mat3(1.0);
}

inline const re::vec3 reProxyShape::randomPoint() const {
  return _transform.multPoint(_shape->randomPoint());
}

inline re::Plane::Location reProxyShape::locationInPlane(const re::Plane& plane) const {
  reTransform inv = re::inverse(_transform);
  return _shape->locationInPlane(re::Plane::apply(inv, plane));
}

#endif
