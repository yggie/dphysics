/**
 * @file
 * Contains the definition of the reTreeBalanceStrategy class
 */
#ifndef RE_TREE_BALANCE_STRATEGY
#define RE_TREE_BALANCE_STRATEGY

#include "react/math.h"
#include "react/Collision/Shapes/Plane.h"
#include "react/Utilities/reLinkedList.h"

class reEnt;
class reBSPNode;

class reTreeBalanceStrategy {
public:
  reTreeBalanceStrategy();
  
  bool shouldMerge(const reBSPNode& node);
  bool shouldSplit(const reBSPNode& node);
  
  re::Plane computeSplitPlane(const re::vec3& axis, const reLinkedList<reEnt*>& sample);
};

inline reTreeBalanceStrategy::reTreeBalanceStrategy() {
  // do nothing
}

#endif
