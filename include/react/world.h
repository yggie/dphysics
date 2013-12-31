#ifndef RE_WORLD_H
#define RE_WORLD_H

#include <vector>

#include "react/common.h"

namespace re {
  
  class RigidBody;
  class Entity;
  class Factory;
  class AbsAllocator;
  
  /**
   * Represents a physical world
   */
  
  class World {
  public:
    World();
    ~World();
    
    void clear();
    
    void step(reFloat dt);
    
    AbsAllocator& allocator();
    Factory factory();
    void add(Entity& entity);
  
  protected:
    std::vector<RigidBody*> _bodies;
    
    AbsAllocator* _allocator;
  };
  
  /**
   * Returns the general purpose memory allocator used by the world object
   * 
   * @return The memory allocator used
   */
  
  inline AbsAllocator& World::allocator() {
    return *_allocator;
  }
}

#endif
