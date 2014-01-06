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
  _size++;
}

void reKDTree::remove(reEnt* ent) {
  // do nothing
}

void reKDTree::split() {
  const reUInt orien = orientation();
  reFloat average = 0.0;
//  for_each(_entities, {
//    average += ent.pos()[orientation()];
//  });
  
  average /= _size;
  const reFloat delta = (average - _pos[orien]) / 2.0;
  
  // allocate new instances
  _childA = _world->allocator().alloc_new<reKDTree>();
  _childB = _world->allocator().alloc_new<reKDTree>();
  
  // setup each child node
  _childA->_pos = _pos;
  _childA->_aabb.set(_aabb);
  _childA->setOrientation((orient + 1) % 3);
  _childA->_pos[orien] += _aabb.dimens()[orien] / 2.0 + delta;
  _childA->_aabb.dimens()[orien] -= delta;
  
  _childB->_pos = _pos;
  _childB->_aabb.set(_aabb);
  _childB->setOrientation((orient + 1) % 3);
  _childA->_pos[orien] = average / 2.0;
  _childA->_aabb.dimens()[orien] += delta;
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
  _world->allocator().alloc_delete<reKDTree>(_childA);
  _world->allocator().alloc_delete<reKDTree>(_childB);
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

