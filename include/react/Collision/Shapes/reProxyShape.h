/**
 * @file
 * Contains the definition of the reProxyShape class
 */
#ifndef RE_TRANSFORMEDSHAPE_H
#define RE_TRANSFORMEDSHAPE_H

#include "react/Collision/Shapes/reShape.h"

class reWorld;

/**
 * A special wrapper class around an arbitrary shape which allows the
 * user to apply additional transformations on the shape, such as scaling
 * or rotating
 */

class reProxyShape : public reShape {
public:
  reProxyShape(const reWorld* world = nullptr);
  reProxyShape(const reProxyShape&) = delete;
  ~reProxyShape();
  
  reProxyShape& operator=(const reProxyShape&) = delete;
  
  const reShape& shape() const;
  void setShape(const reShape& base);
  reProxyShape& withShape(const reShape& base);
  
  const reTransform& transform() const;
  void setTransform(const reTransform& transform);
  reProxyShape& withTransform(const reTransform& transform);
  
  // shape representation
  reShape::Type type() const override;
  reUInt numVerts() const override;
  const reVector vert(reUInt i) const override;
  reFloat shell() const override;
  const reVector offset() const override;
  
  // physical metrics
  reFloat volume() const override;
  const reMatrix computeInertia() const override;
  
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;
  
  bool intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const override;
  
  bool intersectsHyperplane(const reTransform& transform, const reHyperplaneQuery& query) const override;
  
private:
  const reWorld* _world;
  reTransform _transform;
  reShape* _shape;
};

inline void reProxyShape::setTransform(const reTransform& transform) {
  _transform = transform;
}

inline reProxyShape& reProxyShape::withTransform(const reTransform& transform) {
  setTransform(transform);
  return *this;
}

inline const reShape& reProxyShape::shape() const {
  return *_shape;
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

inline const reVector reProxyShape::vert(reUInt i) const {
  if (_shape != nullptr) {
    return _shape->vert(i);
  } else {
    return reVector(0.0, 0.0, 0.0);
  }
}

inline reFloat reProxyShape::shell() const {
  if (_shape != nullptr) {
    return _shape->shell();
  } else {
    return reShape::shell();
  }
}

inline const reVector reProxyShape::offset() const {
  if (_shape != nullptr) {
    return _shape->offset() + _transform.v;
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

inline const reMatrix reProxyShape::computeInertia() const {
//  RE_NOT_IMPLEMENTED
  return reMatrix(1.0);
}
  
inline bool reProxyShape::intersectsHyperplane(const reTransform& transform, const reHyperplaneQuery& query) const {
  return reShape::intersectsHyperplane(transform * _transform, query);
}

#endif
