#include "react/Collision/Shapes/ShapeProxy.h"

#include "react/Collision/Shapes/shapes.h"

using namespace re;

ShapeProxy::ShapeProxy(reShape* shape) : _transform(), _shape(shape) {
  // do nothing
}

ShapeProxy::ShapeProxy(reShape* shape, const re::Transform& transform) : _transform(transform), _shape(shape) {
  // do nothing
}

ShapeProxy::~ShapeProxy() {
  // do nothing
}

bool ShapeProxy::containsPoint(const re::vec3& point) const {
  if (_shape != nullptr) {
    return _shape->containsPoint(re::inverse(_transform), point);
  }
  return false;
}

