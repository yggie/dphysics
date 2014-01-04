#include "react/reDistortedShape.h"

#include "react/reWorld.h"
#include "react/reSphere.h"
#include "react/reTriangle.h"

namespace {
  reShape* allocNewShape(const reShape& shape) {
    switch (shape.type()) {
      case reShape::SPHERE:
        return new reSphere((const reSphere&)shape);
      
      case reShape::RECTANGLE:
        break;
      
      case reShape::COMPOUND:
        break;
      
      case reShape::TRIANGLE:
        return new reTriangle((const reTriangle&)shape);
      
      case reShape::DISTORTED:
        return new reDistortedShape((const reDistortedShape&)shape);
    }
    
    RE_IMPOSSIBLE
    return new reSphere(1.0f);
  }
}

reDistortedShape::reDistortedShape(const reDistortedShape& other)
  : _distortion(other._distortion), _shape(nullptr),
    _world(other._world) {
  if (other._shape != nullptr) {
    setBaseShape(*other._shape);
  }
}

reDistortedShape::reDistortedShape(const reShape& base)
  : _distortion(), _shape(nullptr), _world(nullptr) {
  setBaseShape(base);
}

reDistortedShape::~reDistortedShape() {
  if (_shape != nullptr) {
    if (_world != nullptr) {
      _world->remove(*_shape);
    } else {
      delete _shape;
    }
  }
}

void reDistortedShape::setBaseShape(const reShape& base) {
  if (_shape != nullptr && _world != nullptr) {
    _world->remove(*_shape);
  }
  if (_world != nullptr) {
    _shape = &_world->copyOf(base);
  } else {
    _shape = allocNewShape(base);
  }
}

reDistortedShape& reDistortedShape::withBaseShape(const reShape& base) {
  setBaseShape(base);
  return *this;
}

void reDistortedShape::setWorld(reWorld& world) {
  RE_ASSERT(_world == nullptr, "Cannot be not null")
  reShape* tmp = _shape;
  _shape = &world.copyOf(*tmp);
  delete tmp;
  _world = &world;
}

reDistortedShape& reDistortedShape::withWorld(reWorld& world) {
  setWorld(world);
  return *this;
}

