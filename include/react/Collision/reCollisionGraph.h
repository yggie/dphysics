/**
 * @file
 * Contains the definition of the reCollisionGraph class
 */
#ifndef RE_COLLISION_GRAPH_H
#define RE_COLLISION_GRAPH_H

/**
 * @ingroup collision
 * A graph representation of all collisions in a moment in time
 */

class reCollisionGraph {
public:
  reCollisionGraph(const reWorld* world);
  ~reCollisionGraph();
  
  void solve();
  
private:
  const reWorld& _world;
};

inline reCollisionGraph::reCollisionGraph(const reWorld* world) : _world(*world) { }

inline reCollisionGraph::~reCollisionGraph() {
  // do nothing
}

inline void reCollisionGraph::solve() {
  // do something
}

#endif
