/**
 * @file
 * Contains the definition for the reWorld class
 */
#ifndef RE_WORLD_H
#define RE_WORLD_H

#include "react/common.h"
#include "react/math.h"
#include "react/Memory/reAllocator.h"
#include "react/Utilities/reBuilder.h"
#include "react/Collision/reSpatialQueries.h"

class reEntList;
class reBroadPhase;
class reEnt;
class reShape;
class reIntegrator;

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
  void add(reEnt& entity);
  void advance(reFloat dt);
  
  // getters
  reEntList& entities() const;
  reAllocator& allocator() const;
  reBroadPhase& broadPhase() const;
  reIntegrator& integrator() const;
  reBuilder build();
  
  // spatial queries
  reEnt* queryWithRay(const re::vec3& from, const re::vec3& direction, re::vec3* intersect = nullptr, re::vec3* normal = nullptr);

private:
  /** The reBroadPhase used in this reWorld */
  reBroadPhase* _broadPhase;
  /** The general purpose reAllocator used in this reWorld */
  reAllocator* _allocator;
  /** The integrator used to integrate the time step for all dynamic objects */
  reIntegrator* _integrator;
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

inline reIntegrator& reWorld::integrator() const {
  return *_integrator;
}

inline reBuilder reWorld::build() {
  return reBuilder(*this);
}

#endif
