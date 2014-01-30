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
  void check(const reEnt& entA, const reEnt& entB);
  void advance(reFloat dt);
  
private:
  const reWorld& _world;
};

inline reCollisionGraph::reCollisionGraph(const reWorld* world) : _world(*world) { }

inline reCollisionGraph::~reCollisionGraph() {
  // do nothing
}

inline void reCollisionGraph::solve() {
  // TODO something smart
}

inline void reCollisionGraph::check(const reEnt&, const reEnt&) {
  // TODO something smart
}

inline void reCollisionGraph::advance(reFloat) {
  // TODO something smart
}

#endif
