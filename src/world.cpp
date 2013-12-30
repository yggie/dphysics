#include "react/world.h"

#include "react/rigidbody.h"
#include "react/factory.h"

using namespace re;

World::World() : _bodies() {
  // do nothing
}

World::~World() {
  clear();
}

void World::clear() {
  std::vector<RigidBody*>::iterator it;
  std::vector<RigidBody*>::iterator itEnd = _bodies.end();
  
  for (it = _bodies.begin(); it != itEnd; it++) {
    delete (*it);
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
      _bodies.push_back(static_cast<RigidBody*>(&entity));
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

