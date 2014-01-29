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
struct reBPMeasure;

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
  virtual void advance(reIntegrator& integrator, reFloat dt) = 0;
  
  // getters
  reEntList& entities();
  const reEntList& entities() const;
  
  // spatial queries
  virtual reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const = 0;
  
  // measurement functions
  virtual void measure(reBPMeasure& measure) const = 0;
  
protected:
  /** The parent reWorld object */
  const reWorld& _world;
private:
  /** The list of reEnt contained in this structure */
  reEntList _entities;
};

/**
 * @ingroup collision
 * Used to obtain metrics to measure the performance of the Broad Phase
 */

struct reBPMeasure {
  reBPMeasure() : entities(0), children(0), leafs(0), references(0), meanDepth(0.0) { }
  /** The number of entities in the structure */
  reUInt entities;
  /** The number of child nodes */
  reUInt children;
  /** The number of leaf nodes */
  reUInt leafs;
  /** The number of references kept in the structure */
  reUInt references;
  /** The mean depth for all leaf nodes */
  reFloat meanDepth;
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
 * @fn void reBroadPhase::advance(reIntegrator& integrator, reFloat dt)
 * Advances the entities forward in time and also resolves all collisions
 * in the process
 * 
 * @param integrator The desired integrator to use
 * @param dt The time step to advance, in user-defined units
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
