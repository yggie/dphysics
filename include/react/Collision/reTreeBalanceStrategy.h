/**
 * @file
 * Contains the definition of the reTreeBalanceStrategy class
 */
#ifndef RE_TREE_BALANCE_STRATEGY
#define RE_TREE_BALANCE_STRATEGY

class reBSPTreeNode;

class reTreeBalanceStrategy {
public:
  reTreeBalanceStrategy();
  
  bool shouldMerge(const reBSPTreeNode& node);
  bool shouldSplit(const reBSPTreeNode& node);
};

inline reTreeBalanceStrategy::reTreeBalanceStrategy() {
  // do nothing
}

#endif
