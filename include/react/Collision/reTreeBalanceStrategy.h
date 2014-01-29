/**
 * @file
 * Contains the definition of the reTreeBalanceStrategy class
 */
#ifndef RE_TREE_BALANCE_STRATEGY
#define RE_TREE_BALANCE_STRATEGY

class reBSPTree;

class reTreeBalanceStrategy {
public:
  reTreeBalanceStrategy();
  
  bool shouldMerge(const reBSPTree& node);
  bool shouldSplit(const reBSPTree& node);
};

inline reTreeBalanceStrategy::reTreeBalanceStrategy() {
  // do nothing
}

#endif
