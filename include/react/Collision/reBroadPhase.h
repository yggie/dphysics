/**
 * @file
 * Contains the definition of the reBroadPhase abstract class
 */
#ifndef RE_BROADPHASE_H
#define RE_BROADPHASE_H

#include "react/common.h"
#include "react/Collision/reSpatialQueries.h"

class reEnt;

/**
 * @ingroup collision
 * An abstract class which describes the interface for a broad phase collision
 * detection system
 */

class reBroadPhase {
public:
  /** Default constructor does nothing */
  reBroadPhase();
  /** Destructor constructor does nothing */
  virtual ~reBroadPhase();

  virtual void clear() = 0;
  virtual bool add(reEnt* ent) = 0;
  virtual bool remove(reEnt* ent) = 0;
  virtual void update() = 0;
  
  virtual bool contains(const reEnt* ent) const = 0;
  
  virtual reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const = 0;
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

#endif
