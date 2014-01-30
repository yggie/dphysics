/**
 * @file
 * Contains the definition of the reBSPTreeNode and reBSPTree class
 */
#ifndef RE_BSPTREE_H
#define RE_BSPTREE_H

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reAABB.h"
#include "react/Utilities/reEntList.h"

/**
 * @ingroup collision
 * Represents a node in the BSP tree
 */

class reBSPTreeNode {
public:
  reBSPTreeNode(const reWorld* world, reUInt depth, re::vec3 dir, re::vec3 anchor);
  ~reBSPTreeNode();
  
  void clear();
  reBSPTreeNode& child(reUInt i) const { return *_child[i]; }
  
  reUInt size() const;
  bool hasChildren() const;
  bool isRoot() const;
  bool isLeaf() const;
  
  void add(reQueryable* q);
  void remove(reQueryable* q);
  bool contains(const reEnt* ent) const;
  reEntList rebalanceNode(reTreeBalanceStrategy& strategy);
  
  // spatial queries
  reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const;
  
  // measurement
  void measure(reBPMeasure& measure) const;
  
  /** The split direction of the node */
  const re::vec3 dir;
  /** A point along the split plane */
  const re::vec3 anchor;
  /** The current depth */
  const reUInt depth;
  
protected:
  reEntList trim();
  void split(reTreeBalanceStrategy& strategy);
  void merge();
  
  void optimalSplit(re::vec3& anchor, re::vec3& dir) const;
  
  /** The parent reWorld object */
  const reWorld& _world;
  /** The list of entities contained in this structure */
  reEntList _entities;
  /** The direct descendents of this node */
  reBSPTreeNode* _child[2];
};

/**
 * @ingroup collision
 * The root of the BSP tree structure which acts as the interface for spatial
 * queries
 */

class reBSPTree : public reBroadPhase, public reBSPTreeNode {
public:
  reBSPTree(const reWorld* world);
  ~reBSPTree();
  
  void clear() override { reBSPTreeNode::clear(); }
  bool add(reEnt* ent) override;
  bool remove(reEnt* ent) override;
  void rebalance(reTreeBalanceStrategy* strategy = nullptr) override;
  void advance(reIntegrator& integrator, reFloat dt) override;
  
  reEntList& entities() override;
  
  // spatial queries
  reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const override { return reBSPTreeNode::queryWithRay(query, result); }
  
  // measurement
  void measure(reBPMeasure& m) const override { reBSPTreeNode::measure(m); }
  
protected:
  /** The structure maintaining collision interactions between entities */
  reCollisionGraph _collisions;
  /** The strategy used to balance the tree */
  reTreeBalanceStrategy _strategy;
};

inline reUInt reBSPTreeNode::size() const {
  return _entities.size();
}

inline bool reBSPTreeNode::hasChildren() const {
  return _child[0] != nullptr;
}

inline bool reBSPTreeNode::isRoot() const {
  return depth == 0;
}

inline bool reBSPTreeNode::isLeaf() const {
  return !hasChildren();
}

inline reEntList& reBSPTree::entities() {
  return _entities;
}

#endif
