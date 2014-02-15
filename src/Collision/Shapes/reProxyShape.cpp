#include "react/Collision/Shapes/reProxyShape.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

reProxyShape::reProxyShape(reShape* shape) : _transform(), _shape(shape) {
  // do nothing
}

reProxyShape::reProxyShape(reShape* shape, const re::Transform& transform) : _transform(transform), _shape(shape) {
  // do nothing
}

reProxyShape::~reProxyShape() {
  // do nothing
}

bool reProxyShape::containsPoint(const re::vec3& point) const {
  if (_shape != nullptr) {
    return _shape->containsPoint(re::inverse(_transform), point);
  }
  return false;
}

bool reProxyShape::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(_transform, query, result);
  }
  return false;
}

