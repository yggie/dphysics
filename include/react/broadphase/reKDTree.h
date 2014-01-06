/**
 * @file
 * Contains the definition of the reKDTree class
 */
#ifndef RE_KDTREE_H
#define RE_KDTREE_H

#include "react/broadphase/reBroadPhase.h"

const reUInt RE_KDTREE_NODE_MAX_SIZE = 5;

/**
 * A simple k-d tree data structure for spatial queries
 */

class reKDTree : public reBroadPhase {
public:
  reKDTree();
  ~reKDTree();
  
  void clear() override;
  void add(reEnt* ent) override;
  void remove(reEnt* ent) override;
  void update();
  
  void rebalance();
  
//  reLinkedList<reEnt*> entities();
  
  reUInt orientation();
  reUInt size() const;
  
protected:
  void split();
  void merge();
  
  void setOrientation(reUInt o);
  
  reWorld* _world;
  reKDTree* _childA;
  reKDTree* _childB;
  reUInt _state;
  reUInt _size;
  reAABB _aabb;
  reVector _pos;
//  reLinkedList<reEnt*> _entities;
};

inline reUInt reKDTree::size() const {
  return _size;
}

#endif
