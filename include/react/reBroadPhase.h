/**
 * @file
 */
#ifndef RE_BROADPHASE_H
#define RE_BROADPHASE_H

/**
 * An abstract class which describes the interface for a broad phase collision
 * detection system
 */

class reBroadPhase {
public:
  reBroadPhase();
  ~reBroadPhase();
  
  virtual void add(reEnt* ent) = 0;
  virtual void remove(reEnt* ent) = 0;
  
private:
  reBroadPhase(const reBroadPhase&) { }
};

#endif
