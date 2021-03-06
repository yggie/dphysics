/**
 * @file
 * Contains the definition for the reWorld class
 */
#ifndef RE_WORLD_H
#define RE_WORLD_H

#include "react/common.h"
#include "react/math.h"
#include "react/Memory/reAllocator.h"
#include "react/Utilities/Builder.h"
#include "react/Collision/reSpatialQueries.h"
#include "react/Utilities/reLinkedList.h"

class reBroadPhase;
class reShape;

namespace re {
  class Entity;
  class Integrator;
}

/**
 * Encapsulates the entire implementation of a physics engine
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
  void add(re::Entity& entity);
  void remove(re::Entity& entity);
  void destroy(re::Entity& entity);
  void advance(reFloat dt);
  
  // getters
  const reLinkedList<re::Entity*>& entities() const;
  reAllocator& allocator() const;
  reBroadPhase& broadPhase() const;
  re::Integrator& integrator() const;
  re::Builder build();
  
  // spatial queries
  re::Entity* queryWithRay(const re::vec3& from, const re::vec3& direction, re::vec3* intersect = nullptr, re::vec3* normal = nullptr);

private:
  /** The reBroadPhase used in this reWorld */
  reBroadPhase* _broadPhase;
  /** The general purpose reAllocator used in this reWorld */
  reAllocator* _allocator;
  /** The integrator used to integrate the time step for all dynamic objects */
  re::Integrator* _integrator;
};

/**
 * Returns the general purpose memory allocator object associated with the
 * reWorld
 * 
 * @return The general purpose reAllocator object
 */

inline reAllocator& reWorld::allocator() const {
  return *_allocator;
}

/**
 * Returns the broad phase collision detection object
 * 
 * @return The reBroadPhase used in the reWorld
 */

inline reBroadPhase& reWorld::broadPhase() const {
  return *_broadPhase;
}

inline re::Integrator& reWorld::integrator() const {
  return *_integrator;
}

inline re::Builder reWorld::build() {
  return re::Builder(*this);
}

#endif
