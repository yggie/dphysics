/**
 * @file
 * Contains the definition for the reWorld class
 */
#ifndef RE_WORLD_H
#define RE_WORLD_H

#include <vector>

#include "react/common.h"

class reRigidBody;
class reEnt;
class reShape;
class reAllocator;
class reDistortedShape;

/**
 * Represents a physical world
 */

class reWorld {
public:
  reWorld();
  ~reWorld();
  
  void clear();
  
  reDistortedShape& newDistortedShape(const reShape& shape);
  reRigidBody& newRigidBody();
  reShape& copyOf(const reShape& shape);
  
  void add(reEnt& entity);
  
  void remove(reShape& shape);
  
  reAllocator& allocator();
  
  void step(reFloat dt);

protected:
  std::vector<reRigidBody*> _bodies;
  
  reAllocator* _allocator;
};

/**
 * Returns the general purpose memory allocator used by the world object
 * 
 * @return The memory allocator used
 */

inline reAllocator& reWorld::allocator() {
  return *_allocator;
}

#endif
