/**
 * @file
 * Contains the definition of the reContactGraph class
 */
#ifndef RE_COLLISION_GRAPH_H
#define RE_COLLISION_GRAPH_H

#include "react/Entities/reEnt.h"
#include "react/Utilities/reLinkedList.h"

/**
 * @ingroup collision
 * Represents the contact interaction between two entities
 */

struct reContactEdge {
  reContactEdge(const reEnt* a, const reEnt* b) : A(a), B(b), age(10) { }
  
  const reEnt* const A;
  const reEnt* const B;
  reInt age;
};

/**
 * @ingroup collision
 * A graph representation of all collisions in a moment in time
 */

class reContactGraph {
public:
  reContactGraph(const reWorld* world);
  ~reContactGraph();
  
  void solve();
  void check(const reEnt& entA, const reEnt& entB);
  void advance();
  
private:
  const reWorld& _world;
  reLinkedList<reContactEdge*> _edges;
};

#endif
