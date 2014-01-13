/**
 * @file
 * Contains the definition of the reBSPTree class
 */
#ifndef RE_BSPTREE_H
#define RE_BSPTREE_H

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reAABB.h"
#include "react/Utilities/reEntList.h"

const reUInt RE_BSPTREE_NODE_MIN_SIZE = 10;
const reUInt RE_BSPTREE_DEPTH_LIMIT   = 5;
const reUInt RE_BSPTREE_SAMPLE_SIZE   = 8;
const reUInt RE_BSPTREE_GUESSES       = 3;

/**
 * @ingroup collision
 * A simple BSP tree data structure for spatial queries
 */

class reBSPTree : public reBroadPhase {
public:
  reBSPTree();
  ~reBSPTree();
  
  void clear() override;
  bool add(reEnt* ent) override;
  bool remove(reEnt* ent) override;
  void update() override;
  
  reUInt size() const;
  
  reEnt* queryWithRay(const reRayQuery& query, reRayQueryResult& result) const override;
  
protected:
  void add(reQueryable* q);
  void remove(reQueryable* q);
  
  bool contains(const reEnt* ent) const;
  
  reEntList updateNode();
  reEntList trim();
  void split();
  void merge();
  bool hasChildren() const;
  
  void optimalSplit(reVector& anchor, reVector& dir) const;
  
  reBSPTree* _child[2];
  reUInt _size;
  reVector _dir;
  reVector _anchor;
  reEntList _entities;
  reUInt _depth;
};

inline reUInt reBSPTree::size() const {
  return _size;
}

inline bool reBSPTree::hasChildren() const {
  return _child[0] != nullptr;
}

#endif
