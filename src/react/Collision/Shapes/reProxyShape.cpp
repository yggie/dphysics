#include "react/Collision/Shapes/reProxyShape.h"

#include "react/reWorld.h"
#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/shapes.h"

namespace {
  reShape* allocNewShape(const reShape& base) {
    switch (base.type()) {
      case reShape::SPHERE:
        return new reSphere((const reSphere&)base);
        break;
        
      default:
        RE_WARN("Unexpected shape type\n")
        break;
    }
    
    return nullptr;
  }
}

reProxyShape::reProxyShape(const reWorld* world)
  : _world(world), _transform(), _shape(nullptr) {
  // do nothing
}

reProxyShape::~reProxyShape() {
  if (_shape != nullptr) {
    if (_world == nullptr) {
      delete _shape;
    } else {
      _world->allocator().alloc_delete(_shape);
    }
  }
}

void reProxyShape::setShape(const reShape& base) {
  if (_world == nullptr) {
    if (_shape != nullptr) {
      delete _shape;
    }
    _shape = allocNewShape(base);
  } else {
    if (_shape != nullptr) {
      _world->allocator().alloc_delete(_shape);
    }
    _shape = &_world->copyOf(base);
  }
}

reProxyShape& reProxyShape::withShape(const reShape& base) {
  setShape(base);
  return *this;
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

