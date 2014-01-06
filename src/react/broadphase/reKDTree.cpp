#include "react/broadphase/reKDTree.h"

reKDTree:reKDTree() : _childA(nullptr), _childB(nullptr), _size(0), _entities{nullptr} {
  // do nothing
}

reKDTree::~reKDTree() {
  // do nothing
}

void reKDTree::add(reEnt* ent) {
  if (ent == nullptr) {
    return;
  }
}

void reKDTree::remove(reEnt* ent) {
  // do nothing
}

void reKDTree::split() {
  reFloat average = 0.0;
//  for_each(_entities, {
//    average += ent.pos()[orientation()];
//  });
  
  average /= _size;
  
  _childA = _world->allocator().alloc_new<reKDTree>();
  _childB = _world->allocator().alloc_new<reKDTree>();
//  _childA->_pos.set
//  _childA->_aabb.set()(_aabb, average, orientation());
//  _childB->makeNewAABB(_aabb, average, -orientation());
//  
//  for_each(_entities, {
//    if (ent.intersects(_childA)) {
//      _childA->add(ent);
//    }
//    if (ent.intersects(_childB)) {
//      _childB->add(ent);
//    }
//  });
}

void reKDTree::merge() {
//  _entities.append(_childA->entities());
//  _entities.append(_childB->entities());
  _childA = nullptr;
  _childB = nullptr;
}

void reKDTree::update() {
  if (_childA != nullptr) {
    if (_childA->_size + _childB->_size < RE_KDTREE_NODE_MAX_SIZE) {
      merge();
    }
  } else if (_size > RE_KDTREE_NODE_MAX_SIZE) {
    split();
  }
}

