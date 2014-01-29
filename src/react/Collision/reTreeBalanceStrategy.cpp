#include "react/Collision/reTreeBalanceStrategy.h"

#include "react/Collision/reBSPTree.h"

bool reTreeBalanceStrategy::shouldMerge(const reBSPTree& node) {
  if (!node.hasChildren()) return false;
  return node.child(0).size() + node.child(1).size() < 10;
}

bool reTreeBalanceStrategy::shouldSplit(const reBSPTree& node) {
  return node.size() > 10 && node.depth() < 5;
}
