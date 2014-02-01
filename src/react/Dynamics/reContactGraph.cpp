#include "react/Dynamics/reContactGraph.h"

#include "react/Collision/Shapes/shapes.h"

namespace {
  const reUInt LIMIT = 10;
}

reContactEdge::reContactEdge(reAllocator& allocator, reEnt& a, reEnt& b) : A(a), B(b), contact(false), contactPoint(), contactNormal(), timeLimit(0), interactions(allocator) {
  check();
}

void reContactEdge::check() {
  if (timeLimit++ == 0) {
    timeLimit = LIMIT;
  }
  // TODO TEMPORARY
  const reSphere& sA = (const reSphere&)(*A.shape());
  const reSphere& sB = (const reSphere&)(*B.shape());
  
  const re::vec3 cA = A.center();
  const re::vec3 cB = B.center();
  const reFloat minDist = sA.radius() + sB.radius();
  contact = (re::lengthSq(cA - cB) < minDist*minDist);
  if (contact) {
    contactPoint = (cA + cB) / 2.0;
    contactNormal = re::normalize(cA - cB);
  }
}

/**
 * Creates a new contact graph
 */

reContactGraph::reContactGraph(reAllocator& allocator) : _allocator(allocator), _edges(allocator) {
  // do nothing
}

/**
 * Destructor releases resources used by all contact edges in the graph
 */

reContactGraph::~reContactGraph() {
  for (reContactEdge* edge : _edges) {
    for (reInteraction* action : edge->interactions) {
      _allocator.alloc_delete(action);
    }
    _allocator.alloc_delete(edge);
  }
  _edges.clear();
}

/**
 * Takes each contact edge and solves for the reaction impulses for the contacts
 */

void reContactGraph::solve() {
  const reFloat epsilon = 0.9;
  // TODO TEMPORARY
  for (reContactEdge* edge : _edges) {
    if (edge->contact) {
      reEnt& A = edge->A;
      reEnt& B = edge->B;
      
      const re::vec3 dA = edge->contactPoint - A.center();
      const re::vec3 rA = dA - re::dot(dA, edge->contactNormal) * dA;
      const re::vec3 dB = edge->contactPoint - B.center();
      const re::vec3 rB = dB - re::dot(dB, edge->contactNormal) * dB;
      const re::vec3 rAxN = re::cross(rA, edge->contactNormal);
      const re::vec3 rBxN = re::cross(rB, edge->contactNormal);
      
      const re::vec3 inerA = A.inertiaInv() * rAxN;
      const re::vec3 inerB = B.inertiaInv() * rBxN;
      
      const reFloat numer = -(1 + epsilon) *
                            (re::dot(edge->contactNormal, A.vel() - B.vel()) +
                            (re::dot(A.angVel(), rAxN) - re::dot(B.angVel(), rBxN)));
      
      // contact forces must always be repelling
      if (numer < 0.0) continue;
      
      const reFloat f = numer / (A.massInv() + B.massInv() + re::dot(rAxN, inerA) + re::dot(rBxN, inerB));
      
      const re::vec3 impulse = f * edge->contactNormal;
      
      A.addImpulse(impulse);
      B.addImpulse(-impulse);
//      A.applyRotImpulse(impulse * rAxN);
//      B.applyRotImpulse(impulse * inerB);
    }
    for (reInteraction* action : edge->interactions) {
      action->solve(edge->A, edge->B);
    }
  }
}

/**
 * Checks the contact between the two entities and updates the contact graph
 * 
 * @param A The first entity
 * @param B The second entity
 */

void reContactGraph::check(reEnt& A, reEnt& B) {
  // sanity check
  RE_ASSERT(A.id() < B.id());
  
  // TODO contact filters
  
  for (reContactEdge* edge : _edges) {
    if (edge->A.id() == A.id() && edge->B.id() == B.id()) {
      edge->check();
      return;
    }
  }
  
  // the edge does not exist, create a new one
  reContactEdge* edge = _allocator.alloc_new<reContactEdge>(_allocator, A, B);
  _edges.add(edge);
}

/**
 * Advances the collision graph forward in time. For the collision graph, this
 * only means rejecting contact edges which have expired.
 */

void reContactGraph::advance() {
  reLinkedList<reContactEdge*> toRemove(_allocator);
  // checks for rejected edges
  for (reContactEdge* edge : _edges) {
    if (edge->timeLimit != 0) edge->timeLimit--;
    if (edge->timeLimit == 0 && edge->interactions.empty()) {
      toRemove.add(edge);
    }
  }
  
  // removes all rejected edges
  for (reContactEdge* edge : toRemove) {
    _edges.remove(edge);
    _allocator.alloc_delete(edge);
  }
}

void reContactGraph::addInteraction(reInteraction& action, reEnt& A, reEnt& B) {
  if (A.id() > B.id()) {
    addInteraction(action, B, A);
    return;
  }
  
  for (reContactEdge* edge : _edges) {
    if (edge->A.id() == A.id() && edge->B.id() == B.id()) {
      edge->interactions.add(&action);
      return;
    }
  }
  
  // the edge does not exist, create a new one
  reContactEdge* edge = _allocator.alloc_new<reContactEdge>(_allocator, A, B);
  edge->timeLimit = 0;
  edge->interactions.add(&action);
  _edges.add(edge);
}

