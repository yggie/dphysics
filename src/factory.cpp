#include "react/factory.h"

#include "react/world.h"
#include "react/rigidbody.h"

using namespace re;

Factory::Factory(World& world) : _world(&world) {
  
}

Factory::~Factory() {
  // do nothing
}

Entity* Factory::make() {
  switch (_type) {
    case Entity::RIGID:
      {
        RigidBody* body = new RigidBody();
        _world->add(*body);
        return body;
      }
    
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
  
  return nullptr;
}

