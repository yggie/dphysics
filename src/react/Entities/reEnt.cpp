#include "react/Entities/reEnt.h"

reUInt reEnt::globalEntID = 1;

reEnt::reEnt(reWorld* world) : userdata(nullptr), _world(*world), _id(globalEntID++), _shape(nullptr), _transform() {
  // do nothing
}

reEnt::~reEnt() {
  if (_shape != nullptr) {
    _world.allocator().alloc_delete(_shape);
  }
}
