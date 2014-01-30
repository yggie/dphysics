/**
 * @file
 * Contains the definition of the reTreeBalanceStrategy class
 */
#ifndef RE_TREE_BALANCE_STRATEGY
#define RE_TREE_BALANCE_STRATEGY

#include "react/math.h"
#include "react/Utilities/reEntList.h"

class reBSPTreeNode;

class reTreeBalanceStrategy {
public:
  reTreeBalanceStrategy();
  
  bool shouldMerge(const reBSPTreeNode& node);
  bool shouldSplit(const reBSPTreeNode& node);
  
  void computeSplitPlane(const re::vec3& parentDir, const reEntList& entities, re::vec3& anchor, re::vec3& dir);
};

inline reTreeBalanceStrategy::reTreeBalanceStrategy() {
  // do nothing
}

#endif
