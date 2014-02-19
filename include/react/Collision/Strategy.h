/**
 * @file
 * Contains the definition of the re::Strategy class
 */
#ifndef RE_STRATEGY_H
#define RE_STRATEGY_H

#include "react/math.h"
#include "react/Collision/Shapes/Plane.h"
#include "react/Utilities/reLinkedList.h"

class reBSPNode;

namespace re {
  class Strategy {
  public:
    Strategy();
    
    bool shouldMerge(const reBSPNode& node);
    bool shouldSplit(const reBSPNode& node);
    
    Plane computeSplitPlane(const re::vec3& axis, const reLinkedList<Entity*>& sample);
  };

  inline Strategy::Strategy() {
    // do nothing
  }
}

#endif
