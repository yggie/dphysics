/**
 * @file
 * Contains the definition of the reBroadPhase abstract class
 */
#ifndef RE_BROADPHASE_H
#define RE_BROADPHASE_H

/**
 * @ingroup broadphase
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
  virtual void add(reEnt* ent) = 0;
  virtual void remove(reEnt* ent) = 0;
};

reBroadPhase::reBroadPhase() {
  // do nothing
}

reBroadPhase::~reBroadPhase() {
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
 * @fn void reBroadPhase::add(reEnt* ent)
 * Adds the reEnt into broad phase
 * 
 * @param ent The reEnt to add
 */

/**
 * @fn void reBroadPhase::remove(reEnt* ent)
 * Removes the reEnt from the broad phase
 * 
 * @param ent The reEnt to remove
 */

#endif
