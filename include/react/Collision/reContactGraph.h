/**
 * @file
 * Contains the definition of the reContactEdge and reContactGraph classes
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
  reContactEdge(reEnt* a, reEnt* b);
  
  void check();
  
  reEnt& A;
  reEnt& B;
  bool contact;
  re::vec3 contactPoint;
  re::vec3 contactNormal;
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
  void check(reEnt& entA, reEnt& entB);
  void advance();
  
private:
  const reWorld& _world;
  reLinkedList<reContactEdge*> _edges;
};

inline reContactEdge::reContactEdge(reEnt* a, reEnt* b) : A(*a), B(*b), age(10) {
  check();
}

#endif
