#include "react/Collision/Shapes/reDistortedShape.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

reDistortedShape::reDistortedShape(const reDistortedShape& other)
  : _distortion(other._distortion), _shape(nullptr) {
  if (other._shape != nullptr) {
    setBaseShape(*other._shape);
  }
}

reDistortedShape::reDistortedShape(const reShape& base)
  : _distortion(), _shape(nullptr) {
  setBaseShape(base);
}

reDistortedShape::~reDistortedShape() {
  if (_shape != nullptr) {
    re::alloc_delete(_shape);
  }
}

void reDistortedShape::setBaseShape(const reShape& base) {
  if (_shape != nullptr) {
    re::alloc_delete(_shape);
  }
  _shape = re::copyOf(base);
}

reDistortedShape& reDistortedShape::withBaseShape(const reShape& base) {
  setBaseShape(base);
  return *this;
}

bool reDistortedShape::intersectsRay(const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(_distortion, origin, dir, intersect, normal);
  }
  
  return false;
}

bool reDistortedShape::intersectsRay(const reTMatrix& transform, const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(transform * _distortion, origin, dir, intersect, normal);
  }
  
  return false;
}

