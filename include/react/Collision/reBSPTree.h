/**
 * @file
 * Contains the definition of the reBSPNode and reBSPTree class
 */
#ifndef RE_BSPTREE_H
#define RE_BSPTREE_H

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reAABB.h"
#include "react/Utilities/reEntList.h"

class reBSPNode;
class reBSPTree;

typedef bool(*reBSPTreeCallback)(reBSPNode& node);

/**
 * @ingroup collision
 * Represents a node in the BSP tree
 */

class reBSPNode {
public:
  
  struct Marker {
    Marker(reEnt& e) : entity(e), node(nullptr), queryID(0) { }
    
    reEnt& entity;
    reBSPNode* node;
    reUInt queryID;
  };
  
  reBSPNode(reAllocator& allocator, reUInt depth);
  ~reBSPNode();
  
  void clear();
  const reBSPNode& child(reUInt i) const;
  
  bool remove(Marker& marker);
  
  bool hasChildren() const;
  bool isRoot() const;
  bool isLeaf() const;
  
  reAllocator& allocator() const;
  reUInt depth() const;
  reUInt placements() const;
  
  void rebalanceNode(reTreeBalanceStrategy& strategy);
  void updateContacts(reContactGraph& collisions, reEnt& entity) const;
  
  void measureRecursive(reBPMeasure& m) const;
  
  bool execute(reBSPTreeCallback callback);
  void split(reTreeBalanceStrategy& strategy);
  void merge();
  
  const reLinkedList<reEnt*> sample(reUInt size) const;
  
  // spatial queries
  void queryWithRay(const reRayQuery& query, re::RayResult& result) const;

  reBSPNode* place(Marker& marker);
  
protected:
  /** The allocator object used for allocating memory */
  reAllocator& _allocator;
  /** The list of entities contained in this structure */
  reLinkedList<Marker*> _markers;
  /** The direct descendents of this node */
  reBSPNode* _children[2];
  /** The split plane of the node */
  re::Plane _splitPlane;
  /** The current depth */
  const reUInt _depth;
};

/**
 * @ingroup collision
 * The root of the BSP tree structure which acts as the interface for spatial
 * queries
 */

class reBSPTree : public reBroadPhase, public reBSPNode {
public:
  reBSPTree(reAllocator& allocator);
  ~reBSPTree();
  
  void clear() override;
  bool add(reEnt& ent) override;
  bool remove(reEnt& ent) override;
  bool contains(const reEnt& ent) const override;
  reUInt size() const override;
  void rebalance(reTreeBalanceStrategy* strategy = nullptr) override;
  void advance(reIntegrator& integrator, reFloat dt) override;
  
  void addInteraction(reInteraction& action, reEnt& A, reEnt& B) override;
  
  const reLinkedList<reEnt*>& entities() const override;
  
  // spatial queries
  re::RayResult queryWithRay(const reRayQuery& query) const override;
  
  // measurement
  reBPMeasure measure() const override;
  
protected:
  
  /** The structure maintaining collision interactions between entities */
  reContactGraph _contacts;
  /** The strategy used to balance the tree */
  reTreeBalanceStrategy _strategy;
  reLinkedList<Marker*> _allMarkers;
  reLinkedList<reEnt*> _masterEntityList;
};

inline const reBSPNode& reBSPNode::child(reUInt i) const {
  return *_children[i];
}

inline bool reBSPNode::hasChildren() const {
  return _children[0] != nullptr;
}

inline bool reBSPNode::isRoot() const {
  return _depth == 0;
}

inline bool reBSPNode::isLeaf() const {
  return !hasChildren();
}

inline reAllocator& reBSPNode::allocator() const {
  return _allocator;
}

inline reUInt reBSPNode::depth() const {
  return _depth;
}

inline reUInt reBSPNode::placements() const {
  return _markers.size();
}

inline const reLinkedList<reEnt*>& reBSPTree::entities() const {
  return _masterEntityList;
}

inline reUInt reBSPTree::size() const {
  return _masterEntityList.size();
}

inline re::RayResult reBSPTree::queryWithRay(const reRayQuery& query) const {
  re::RayResult result;
  reBSPNode::queryWithRay(query, result);
  return result;
}

inline reBPMeasure reBSPTree::measure() const {
  reBPMeasure m;
  m.entities = _masterEntityList.size();
  reBSPNode::measureRecursive(m);
  m.children--; // account for the root node
  m.meanLeafDepth /= m.leafs;
  return m;
}

#endif
