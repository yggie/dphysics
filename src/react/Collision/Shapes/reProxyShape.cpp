#include "react/Collision/Shapes/reProxyShape.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

reProxyShape::reProxyShape(reShape* shape, const reTransform* transform) : _transform(*transform), _shape(shape) {
  // do nothing
}

reProxyShape::~reProxyShape() {
  // do nothing
}

bool reProxyShape::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(_transform, query, result);
  }
  return false;
}

bool reProxyShape::intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(transform * _transform, query, result);
  }
  
  return false;
}

