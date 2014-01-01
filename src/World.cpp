#include "react/World.h"

#include "react/memory/ProxyAllocator.h"
#include "react/memory/FreeListAllocator.h"

#include "react/RigidBody.h"
#include "react/Sphere.h"

using namespace re;

const int MEM_ALLOC = 1024*5;
//const int MEM_ALLOC = 350;

char buffer[MEM_ALLOC];

World::World() : _bodies() {
  FreeListAllocator* tmp = new FreeListAllocator(MEM_ALLOC, &buffer[0]);
  _allocator = new ProxyAllocator(tmp);
}

World::~World() {
  clear();
  
  RE_ASSERT(_allocator != nullptr, "No allocator was defined for the world object")
  if (_allocator != nullptr) {
    delete _allocator;
    _allocator = nullptr;
  }
}

void World::clear() {
  std::vector<RigidBody*>::iterator it;
  std::vector<RigidBody*>::iterator itEnd = _bodies.end();
  
//  ProxyAllocator* proxy = (ProxyAllocator*)_allocator;
  for (it = _bodies.begin(); it != itEnd; it++) {
//    proxy->show();
    _allocator->alloc_delete<Shape>((*it)->shape());
    _allocator->alloc_delete<Ent>(*it);
  }
  
  _bodies.clear();
}

RigidBody& World::newRigidBody() {
  RigidBody* body = _allocator->alloc_new<RigidBody>();
  add(*body);
  return *body;
}

Shape& World::copyOf(const Shape& shape) {
  switch (shape.type()) {
    case Shape::SPHERE:
      return *_allocator->alloc_new<Sphere>((const Sphere&)shape);
    
    case Shape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case Shape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
  }
  
  RE_IMPOSSIBLE
  
  return *_allocator->alloc_new<Sphere>(1.0);
}

/**
 * Registers the input Ent to the World's engine.
 * 
 * @param entity The entity to attach
 */

void World::add(Ent& entity) {
  
  RE_ASSERT_WARN(entity.world() == nullptr, "The entity was already registered to another world")
  
  entity.setWorld(this);
  switch (entity.type()) {
    case Ent::RIGID:
      _bodies.push_back((RigidBody*)(&entity));
      break;
    
    case Ent::STATIC:
      RE_NOT_IMPLEMENTED
      break;
    
    case Ent::FIELD:
      RE_NOT_IMPLEMENTED
      break;
    
    case Ent::PARTICLE:
      RE_NOT_IMPLEMENTED
      break;
    
    default:
      RE_IMPOSSIBLE
      break;
  }
}

void World::step(reFloat dt) {
  // do nothing
  dt += 1;
}

