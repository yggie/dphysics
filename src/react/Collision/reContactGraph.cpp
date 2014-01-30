#include "react/Collision/reContactGraph.h"

reContactGraph::reContactGraph(const reWorld* world) : _world(*world), _edges(world) {
  // do nothing
}

reContactGraph::~reContactGraph() {
  for (reContactEdge* edge : _edges) {
    _world.allocator().alloc_delete(edge);
  }
  _edges.clear();
}

void reContactGraph::solve() {
  // TODO something smart
}

/**
 * Checks the contact between the two entities and updates the contact graph
 */

void reContactGraph::check(const reEnt& A, const reEnt& B) {
  for (reContactEdge* edge : _edges) {
    if (edge->A->id() == A.id() && edge->B->id() == B.id()) {
      edge->age++;
      return;
    }
  }
  reContactEdge* edge = _world.allocator().alloc_new<reContactEdge>(&A, &B);
  _edges.add(edge);
}

/**
 * Advances the collision graph forward in time. For the collision graph, this
 * only means rejecting contact edges which have expired.
 */

void reContactGraph::advance() {
  reLinkedList<reContactEdge*> toRemove(&_world);
  for (reContactEdge* edge : _edges) {
    if (--edge->age <= 0) {
      toRemove.add(edge);
    }
  }
  for (reContactEdge* edge : toRemove) {
    _edges.remove(edge);
    _world.allocator().alloc_delete(edge);
  }
}
