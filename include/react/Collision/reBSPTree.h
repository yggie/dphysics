/**
 * @file
 * Contains the definition of the reBSPTree class
 */
#ifndef RE_BSPTREE_H
#define RE_BSPTREE_H

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reAABB.h"
#include "react/Utilities/reLinkedList.h"

const reUInt RE_KDTREE_NODE_MAX_SIZE = 20;

/**
 * @ingroup collision
 * A simple k-d tree data structure for spatial queries
 */

class reBSPTree : public reBroadPhase {
public:
  reBSPTree();
  ~reBSPTree();
  
  void clear() override;
  bool add(reEnt* ent) override;
  bool remove(reEnt* ent) override;
  void update() override;
  
  reLinkedList<reEnt*> entities();
  
  bool contains(const reEnt* ent) const override;
  
  reUInt size() const;
  
  reEnt* queryWithRay(
    const reVector& from,
    const reVector& direction,
    reVector* intersect = nullptr,
    reVector* normal = nullptr
  ) const override;
  
protected:
  reLinkedList<reEnt*> updateNode();
  reLinkedList<reEnt*> trim();
  void split();
  void merge();
  void resizeToFit();
  void show();
  
  bool hasChildren() const;
  
  reBSPTree* _child[2];
  reUInt _size;
  reVector _dir;
  reVector _point;
  reLinkedList<reEnt*> _entities;
  reUInt _depth;
};

inline reUInt reBSPTree::size() const {
  return _size;
}

inline bool reBSPTree::hasChildren() const {
  return _child[0] != nullptr;
}

#endif
