#ifndef RE_WORLD_H
#define RE_WORLD_H

#include <vector>

#include "react/common.h"

namespace re {
  
  class RigidBody;
  class Entity;
  class Factory;
  
  /**
   * Represents a physical world
   */
  
  class World {
  public:
    World();
    ~World();
    
    void clear();
    
    void step(reFloat dt);
    
    Factory factory();
    void add(Entity& entity);
  
  protected:
    std::vector<RigidBody*> _bodies;
  };
}

#endif
