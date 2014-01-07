/**
 * @file
 * Contains the definition for the reWorld class
 */
#ifndef RE_WORLD_H
#define RE_WORLD_H

#include <vector>

#include "react/common.h"
#include "react/math.h"

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
  
  void add(reEnt* entity);
  
  void step(reFloat dt);
  
  reEnt* shootRay(const reVector& from, const reVector& direction, reVector* intersect = nullptr, reVector* normal = nullptr);

protected:
  void ensureUpdate();
  
  std::vector<reRigidBody*> _bodies;
  reBroadPhase* _broadPhase;
  
  bool _updated;
};

#endif
