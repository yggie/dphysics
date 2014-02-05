/**
 * @file
 * Contains the definition of the reTreeBalanceStrategy class
 */
#ifndef RE_TREE_BALANCE_STRATEGY
#define RE_TREE_BALANCE_STRATEGY

#include "react/math.h"
#include "react/Utilities/reLinkedList.h"

class reEnt;
class reBSPNode;

namespace re {
  struct SplitPlane {
    re::vec3 normal;
    re::vec3 center;
  };
}

class reTreeBalanceStrategy {
public:
  reTreeBalanceStrategy();
  
  bool shouldMerge(const reBSPNode& node);
  bool shouldSplit(const reBSPNode& node);
  
  re::SplitPlane computeSplitPlane(const re::vec3& parentDir, const reLinkedList<reEnt*>& sample);
};

inline reTreeBalanceStrategy::reTreeBalanceStrategy() {
  // do nothing
}

#endif
