#include "react/world.h"

#include "react/rigidbody.h"
#include "react/factory.h"
#include "react/memory/proxyallocator.h"
#include "react/memory/freelistallocator.h"

using namespace re;

char buffer[1024*1024];

World::World() : _bodies() {
  FreeListAllocator* tmp = new FreeListAllocator(1024*1024, &buffer[0]);
  _allocator = new ProxyAllocator(tmp);
}

World::~World() {
  clear();
  
  ProxyAllocator* proxy = (ProxyAllocator*)_allocator;
  BaseAllocator* allocer = proxy->allocator();
  delete _allocator;
  delete allocer;
}

void World::clear() {
  std::vector<RigidBody*>::iterator it;
  std::vector<RigidBody*>::iterator itEnd = _bodies.end();
  
  for (it = _bodies.begin(); it != itEnd; it++) {
    _allocator->alloc_delete(*it);
  }
  
  _bodies.clear();
}

void World::step(reFloat dt) {
  // do nothing
  dt += 1;
}

Factory World::factory() {
  return Factory(*this);
}

void World::add(Entity& entity) {
  switch (entity.type()) {
    case Entity::RIGID:
      _bodies.push_back((RigidBody*)(&entity));
      break;
    
    case Entity::STATIC:
      RE_NOT_IMPLEMENTED
      break;
    
    case Entity::FIELD:
      RE_NOT_IMPLEMENTED
      break;
    
    case Entity::PARTICLE:
      RE_NOT_IMPLEMENTED
      break;
    
    default:
      RE_IMPOSSIBLE
      break;
  }
}

