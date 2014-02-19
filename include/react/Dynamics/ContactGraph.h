/**
 * @file
 * Contains the definition of the ContactEdge and ContactGraph 
 * classes
 */
#ifndef RE_COLLISION_GRAPH_H
#define RE_COLLISION_GRAPH_H

#include "react/Entities/Entity.h"
#include "react/Utilities/reLinkedList.h"
#include "react/Dynamics/reInteraction.h"
#include "react/Utilities/ContactFilter.h"

namespace re {
  /**
   * @ingroup dynamics
   * Represents the contact interaction between two entities
   */

  struct ContactEdge : public re::Intersect {
    ContactEdge(reAllocator& allocator, Entity& a, Entity& b);
    
    void check();
    
    Entity& A;
    Entity& B;
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
    void check(Entity& entA, Entity& entB);
    void advance();
    
    void addInteraction(reInteraction& action, Entity& A, Entity& B);
    
  private:
    reAllocator& _allocator;
    reLinkedList<ContactEdge*> _edges;
    re::ContactFilter _filter;
  };
}

#endif
