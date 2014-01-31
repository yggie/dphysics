/**
 * @file
 * Contains the definition of the reContactEdge and reContactGraph classes
 */
#ifndef RE_COLLISION_GRAPH_H
#define RE_COLLISION_GRAPH_H

#include "react/Entities/reEnt.h"
#include "react/Utilities/reLinkedList.h"
#include "react/Dynamics/reInteraction.h"

/**
 * @ingroup dynamics
 * Represents the contact interaction between two entities
 */

struct reContactEdge {
  reContactEdge(const reWorld* world, reEnt* a, reEnt* b);
  
  void check();
  
  reEnt& A;
  reEnt& B;
  bool contact;
  re::vec3 contactPoint;
  re::vec3 contactNormal;
  reUInt timeLimit;
  reLinkedList<reInteraction*> interactions;
};

/**
 * @ingroup dynamics
 * A graph representation of all collisions in a moment in time
 */

class reContactGraph {
public:
  reContactGraph(const reWorld* world);
  ~reContactGraph();
  
  void solve();
  void check(reEnt& entA, reEnt& entB);
  void advance();
  
  void addInteraction(reInteraction* action, reEnt& A, reEnt& B);
  
private:
  const reWorld& _world;
  reLinkedList<reContactEdge*> _edges;
};

#endif
