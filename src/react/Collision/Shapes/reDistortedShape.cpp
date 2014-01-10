#include "react/Collision/Shapes/reDistortedShape.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

namespace {
  reShape* allocNewShape(const reShape& base) {
    switch (base.type()) {
      case reShape::SPHERE:
        return new reSphere((const reSphere&)base);
        break;
        
      default:
        RE_LOG("DARN")
        break;
    }
    
    return nullptr;
  }
}

reDistortedShape::reDistortedShape(const reShape& base)
  : _distortion(), _shape(nullptr) {
  setShape(base);
}

reDistortedShape::~reDistortedShape() {
  if (_shape != nullptr) {
    delete _shape;
//    re::alloc_delete(_shape);
  }
}

void reDistortedShape::setShape(const reShape& base) {
  if (_shape != nullptr) {
//    re::alloc_delete(_shape);
    delete _shape;
  }
  // TODO use the global allocator
  _shape = allocNewShape(base);
//  _shape = re::copyOf(base);
}

reDistortedShape& reDistortedShape::withShape(const reShape& base) {
  setShape(base);
  return *this;
}

bool reDistortedShape::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(_distortion, query, result);
  }
  return false;
}

bool reDistortedShape::intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(transform * _distortion, query, result);
  }
  
  return false;
}

