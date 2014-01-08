#include "react/Collision/Shapes/reDistortedShape.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

reDistortedShape::reDistortedShape(const reShape& base)
  : _distortion(), _shape(nullptr) {
  setShape(base);
}

reDistortedShape::~reDistortedShape() {
  if (_shape != nullptr) {
    re::alloc_delete(_shape);
    _shape = nullptr;
  }
}

void reDistortedShape::setShape(const reShape& base) {
  if (_shape != nullptr) {
    re::alloc_delete(_shape);
  }
  _shape = re::copyOf(base);
}

reDistortedShape& reDistortedShape::withShape(const reShape& base) {
  setShape(base);
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

