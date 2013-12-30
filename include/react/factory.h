#ifndef RE_FACTORY_H
#define RE_FACTORY_H

#include "react/entity.h"

namespace re {
  
  class World;
  class AbsShape;
  
  /**
   * Simplifies the object creation process. Each instance is associated with
   * a parent World object. Every object created by the factory is attached to
   * the parent World object.
   * 
   * @see World
   */
  
  class Factory {
  public:
    Factory(World& world);
    ~Factory();
    
    Factory& as(Entity::Type type);
    
    Entity* make();
    
  protected:
    World* _world;
    AbsShape* _shape;
    Entity::Type _type;
  };
  
  inline Factory& Factory::as(Entity::Type type) {
    _type = type;
    return *this;
  }
}

#endif
