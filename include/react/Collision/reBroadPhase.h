/**
 * @file
 * Contains the definition of the reBroadPhase abstract class
 */
#ifndef RE_BROADPHASE_H
#define RE_BROADPHASE_H

#include "react/common.h"
#include "react/Utilities/reEntList.h"
#include "react/Collision/reSpatialQueries.h"

class reEnt;
class reWorld;

/**
 * @ingroup collision
 * An abstract class which describes the interface for a broad phase collision
 * detection system. It represents a structure used to accelerate spatial
 * queries to all entities contained within.
 */

class reBroadPhase {
public:
  reBroadPhase(const reWorld* world);
  /** Destructor constructor does nothing */
  virtual ~reBroadPhase() = 0;

  virtual void clear() = 0;
  virtual bool add(reEnt* ent) = 0;
  virtual bool remove(reEnt* ent) = 0;
  virtual void update() = 0;
  
  // getters
  reEntList& entities();
  const reEntList& entities() const;
  
  // spatial queries
  virtual reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const = 0;
  
protected:
  /** The parent reWorld object */
  const reWorld& _world;
private:
  /** The list of reEnt contained in this structure */
  reEntList _entities;
};

/**
 * Initializes the reBroadPhase with the given parent world
 * 
 * @param world The parent reWorld
 */

inline reBroadPhase::reBroadPhase(const reWorld* world) : _world(*world), _entities(world) {
  // do nothing
}

inline reBroadPhase::~reBroadPhase() {
  // do nothing
}

/**
 * Returns the list of entities
 * 
 * @return The reEntList containing all the entities in the reBroadPhase
 */

inline reEntList& reBroadPhase::entities() {
  return _entities;
}

/**
 * Returns the list of entities.
 * 
 * Enforces the constant constraint
 * 
 * @return The reEntList containing all the entities in the reBroadPhase
 */

inline const reEntList& reBroadPhase::entities() const {
  return _entities;
}

/**
 * @fn void reBroadPhase::step(reFloat dt)
 * Updates all the entities attached with the given time step
 * 
 * @param dt The time step in user-defined units
 */

/**
 * @fn void reBroadPhase::clear()
 * Clears the reBroadPhase of references to any reEnt
 */

/**
 * @fn bool reBroadPhase::add(reEnt* ent)
 * Adds the reEnt into structure
 * 
 * @param ent The reEnt to add
 * @return True if the operation was successful
 */

/**
 * @fn void reBroadPhase::remove(reEnt* ent)
 * Removes the reEnt from the broad phase
 * 
 * @param ent The reEnt to remove
 * @return True if the operation was successful
 */

/**
 * @fn void reBroadPhase::update()
 * Notifies the broad phase that the entities positions have changed
 */

/**
 * @fn reEnt* reBroadPhase::queryWithRay(const reRayQuery& query,
 * reRayQueryResult& result) const;
 * Performs a spatial query into the structure using a ray
 * 
 * @param query The ray query structure
 * @param result The ray query result structure
 * @return The reEnt which was found
 */

#endif
