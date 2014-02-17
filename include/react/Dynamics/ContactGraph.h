/**
 * @file
 * Contains the definition of the ContactEdge and ContactGraph 
 * classes
 */
#ifndef RE_COLLISION_GRAPH_H
#define RE_COLLISION_GRAPH_H

#include "react/Entities/reEnt.h"
#include "react/Utilities/reLinkedList.h"
#include "react/Dynamics/reInteraction.h"
#include "react/Utilities/ContactFilter.h"

namespace re {
  /**
   * @ingroup dynamics
   * Represents the contact interaction between two entities
   */

  struct ContactEdge : public re::Intersect {
    ContactEdge(reAllocator& allocator, reEnt& a, reEnt& b);
    
    void check();
    
    reEnt& A;
    reEnt& B;
    bool contact;
    reUInt timeLimit;
    reLinkedList<reInteraction*> interactions;
  };

  /**
   * @ingroup dynamics
   * A graph representation of all collisions in a moment in time
   */

  class ContactGraph {
  public:
    ContactGraph(reAllocator& allocator);
    ~ContactGraph();
    
    void solve();
    void check(reEnt& entA, reEnt& entB);
    void advance();
    
    void addInteraction(reInteraction& action, reEnt& A, reEnt& B);
    
  private:
    reAllocator& _allocator;
    reLinkedList<ContactEdge*> _edges;
    re::ContactFilter _filter;
  };
}

#endif
