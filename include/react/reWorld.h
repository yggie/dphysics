/**
 * @file
 * Contains the definition for the reWorld class
 */
#ifndef RE_WORLD_H
#define RE_WORLD_H

#include "react/common.h"
#include "react/math.h"
#include "react/Collision/reSpatialQueries.h"
#include "react/Memory/reAllocator.h"

class reBroadPhase;
class reRigidBody;
class reEnt;
class reShape;
class reDistortedShape;

/**
 * Represents a physical world
 */

class reWorld {
public:
  reWorld();
  /** Prohibit copying */
  reWorld(const reWorld&) = delete;
  ~reWorld();
  
  /** Prohibit copying */
  reWorld& operator=(const reWorld&) = delete;
  
  void clear();
  
  reDistortedShape& newDistortedShape(const reShape& shape);
  reRigidBody& newRigidBody();
  
  void forEachEntDo(void(*func)(reEnt* ent));
  
  void add(reEnt* entity);
  
  // time update function
  void update(reFloat dt);
  
  reAllocator& allocator() const;
  reBroadPhase& broadPhase() const;
  
  // spatial queries
  reEnt* queryWithRay(const reVector& from, const reVector& direction, reVector* intersect = nullptr, reVector* normal = nullptr);
  
  // memory allocation functions
  reShape& copyOf(const reShape& shape) const;

protected:
  void ensureUpdate();
  
  reBroadPhase* _broadPhase;
  reAllocator* _allocator;
  
  bool _updated;
};

inline reAllocator& reWorld::allocator() const {
  return *_allocator;
}

inline reBroadPhase& reWorld::broadPhase() const {
  return *_broadPhase;
}

#endif
