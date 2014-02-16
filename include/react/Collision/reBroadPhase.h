/**
 * @file
 * Contains the definition of the reBroadPhase abstract class
 */
#ifndef RE_BROADPHASE_H
#define RE_BROADPHASE_H

#include "react/common.h"
#include "react/Utilities/reEntList.h"
#include "react/Dynamics/reInteraction.h"
#include "react/Dynamics/ContactGraph.h"
#include "react/Collision/reSpatialQueries.h"
#include "react/Collision/reTreeBalanceStrategy.h"

class reEnt;
class reBPMeasure;

/**
 * @ingroup collision
 * An abstract class which describes the interface for a broad phase collision
 * detection system. It represents a structure used to accelerate spatial
 * queries to all entities contained within.
 */

class reBroadPhase {
public:
  /** Default constructor does nothing */
  reBroadPhase();
  /** Destructor constructor does nothing */
  virtual ~reBroadPhase() = 0;

  virtual void clear() = 0;
  virtual bool add(reEnt& ent) = 0;
  virtual bool remove(reEnt& ent) = 0;
  virtual bool contains(const reEnt& ent) const = 0;
  virtual reUInt size() const = 0;
  virtual void rebalance(reTreeBalanceStrategy* strategy = nullptr) = 0;
  virtual void advance(re::Integrator& integrator, reFloat dt) = 0;
  
  virtual void addInteraction(reInteraction& action, reEnt& A, reEnt& B) = 0;
  
  virtual const reLinkedList<reEnt*>& entities() const = 0;
  
  // spatial queries
  virtual re::RayQuery queryWithRay(const re::Ray& ray) const = 0;
  
  // measurement functions
  virtual reBPMeasure measure() const = 0;
};

/**
 * @ingroup collision
 * Used to obtain metrics to measure the performance of the Broad Phase
 */

struct reBPMeasure {
  reBPMeasure() : entities(0), children(0), leafs(0), references(0), meanLeafDepth(0.0) { }
  /** The number of entities in the structure */
  reUInt entities;
  /** The number of child nodes */
  reUInt children;
  /** The number of leaf nodes */
  reUInt leafs;
  /** The number of references kept in the structure */
  reUInt references;
  /** The mean depth for all leaf nodes */
  reFloat meanLeafDepth;
};

inline reBroadPhase::reBroadPhase() {
  // do nothing
}

inline reBroadPhase::~reBroadPhase() {
  // do nothing
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
 * @fn void reBroadPhase::rebalance(reTreeBalanceStrategy& strategy)
 * Notifies the broad phase that the entities positions have changed
 * 
 * @param strategy The strategy used to rebalance the tree
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

/**
 * @fn reBPMeasure reBroadPhase::measure()
 * Stores usage data related to the structures and returns it
 * 
 * @return The measure object containing usage data
 */

#endif
