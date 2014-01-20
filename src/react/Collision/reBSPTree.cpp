#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/Shapes/reProxyShape.h"

#include <cstdio>

reBSPTree::reBSPTree(const reWorld* world, reUInt depth) : reBroadPhase(world), _child{nullptr}, _dir(1.0, 0.0, 0.0), _anchor(0.0, 0.0, 0.0), _depth(depth) {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
}

void reBSPTree::clear() {
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      _child[i]->clear();
      _world.allocator().alloc_delete(_child[i]);
      _child[i] = nullptr;
    }
  }
  
  if (isRoot()) {
    auto end = entities().qEnd();
    for (auto iter = entities().qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      RE_EXPECT(q.ent->userdata == nullptr)
      if (q.ent->shape()->type() == reShape::PROXY) {
        _world.allocator().alloc_delete(((reProxyShape*)q.ent->shape())->shape());
      }
      _world.allocator().alloc_delete(q.ent->shape());
      _world.allocator().alloc_delete(q.ent);
      _world.allocator().alloc_delete(&q);
    }
  }
  entities().clear();
}

bool reBSPTree::add(reEnt* ent) {
  if (ent == nullptr) {
    return false;
  }
  
  reQueryable* q = _world.allocator().alloc_new<reQueryable>(ent);
  if (entities().add(q)) {
    if (hasChildren()) {
      for (reUInt i = 0; i < 2; i++) {
        if (_child[i]->contains(ent)) {
          _child[i]->add(q);
        }
      }
    }
    return true;
  } else {
    _world.allocator().alloc_delete(q);
    return false;
  }
}

bool reBSPTree::remove(reEnt* ent) {
  auto end = entities().qEnd();
  for (auto iter = entities().qBegin(); iter != end; ++iter) {
    reQueryable& q = *iter;
    if (q.ent->id() == ent->id()) {
      if (hasChildren()) {
        for (reUInt i = 0; i < 2; i++) {
          if (_child[i]->contains(ent)) {
            _child[i]->remove(&q);
          }
        }
      }
      entities().remove(&q);
      _world.allocator().alloc_delete(&q);
      return true;
    }
  }
  
  return false;
}

void reBSPTree::update() {
  if (hasChildren()) {
    if (_child[0]->size() + _child[1]->size() < RE_BSPTREE_NODE_MIN_SIZE) {
      merge();
    } else {
      reEntList list(&_world);
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->updateNode());
        
        auto end = list.qEnd();
        for (auto iter = list.qBegin(); iter != end; ++iter) {
          reQueryable& q = *iter;
          const reUInt j = (i + 1) % 2;
          if (_child[j]->contains(q.ent)) {
            _child[j]->add(&q);
          }
        }
        
        list.clear();
      }
    }
  } else {
    if (!isRoot()) {
      trim();
    }
    if (size() > RE_BSPTREE_NODE_MIN_SIZE && _depth < RE_BSPTREE_DEPTH_LIMIT) {
      split();
    }
  }
  
  printf("[ROOT] (%3d, %3d) (%+.1f, %+.1f, %+.1f) (%+.1f, %+.1f, %+.1f)\n", _depth, size(), _dir[0], _dir[1], _dir[2], _anchor[0], _anchor[1], _anchor[2]);
}

void reBSPTree::add(reQueryable* q) {
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->contains(q->ent)) {
        _child[i]->add(q);
      }
    }
  } else {
    entities().add(q);
  }
}

void reBSPTree::remove(reQueryable* q) {
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->contains(q->ent)) {
        _child[i]->remove(q);
      }
    }
  } else {
    entities().remove(q);
  }
}

bool reBSPTree::contains(const reEnt* ent) const {
  if (ent == nullptr) {
    RE_WARN("Attempted to query using null\n")
    return false;
  }
  
  return ent->intersectsHyperplane(_anchor, _dir);
}

/**
 * Called from the root or parent node to update its structure. If any reEnts
 * were removed, it is returned
 * 
 * @return A list of rejected entities()
 */

reEntList reBSPTree::updateNode() {
  reEntList list(&_world);
  
  if (hasChildren()) {
    if (_child[0]->size() + _child[1]->size() < RE_BSPTREE_NODE_MIN_SIZE) {
      merge();
      return updateNode();
    } else {
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->updateNode());
        
        auto end = list.qEnd();
        for (auto iter = list.qBegin(); iter != end; ++iter) {
          reQueryable& q = *iter;
          const reUInt j = (i + 1) % 2;
          if (_child[j]->contains(q.ent)) {
            _child[j]->add(&q);
            list.remove(&q);
          }
        }
      }
    }
  } else {
    list.append(trim());
    if (size() > RE_BSPTREE_NODE_MIN_SIZE && _depth < RE_BSPTREE_DEPTH_LIMIT) {
      split();
    }
  }
  
  if (!hasChildren()) {
    printf("[NODE] (%3d, %3d) (%+.1f, %+.1f, %+.1f) (%+.1f, %+.1f, %+.1f)\n", _depth, size(), _dir[0], _dir[1], _dir[2], _anchor[0], _anchor[1], _anchor[2]);
  }
  
  return list;
}

/**
 * Called when a node should verify if all entities() are still contained. 
 * Returns a list of rejected entities()
 * 
 * @return A list of rejected entities()
 */

reEntList reBSPTree::trim() {
  reEntList rejected(&_world);
  auto end = entities().qEnd();
  for (auto iter = entities().qBegin(); iter != end; ++iter) {
    reQueryable& q = *iter;
    if (!contains(q.ent)) {
      remove(&q);
      rejected.add(&q);
    }
  }
  
  return rejected;
}

/**
 * Called when the tree requires branching out
 */

void reBSPTree::split() {
  reVector splitAnchor, splitDir;
  optimalSplit(splitAnchor, splitDir);
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = _world.allocator().alloc_new<reBSPTree>(&_world, _depth + 1);
    _child[i]->_anchor = splitAnchor;
    _child[i]->_dir = splitDir * ((i % 2 == 0) ? -1 : 1);
    
    auto end = entities().qEnd();
    for (auto iter = entities().qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      if (_child[i]->contains(q.ent)) {
        _child[i]->add(&q);
      }
    }
    
    _child[i]->updateNode();
  }
  
  if (!isRoot()) {
    entities().clear();
  }
}

/**
 * Called when the _child nodes are to be merged with the parent
 */

void reBSPTree::merge() {
  for (reUInt i = 0; i < 2; i++) {
    entities().append(_child[i]->entities());
    _world.allocator().alloc_delete<reBSPTree>(_child[i]);
    _child[i] = nullptr;
  }
  printf("[NODE] MERGE %d\n", _depth);
}

reEnt* reBSPTree::queryWithRay(const reRayQuery& query, reRayQueryResult& result) const {
  
  if (!entities().empty() && !isRoot()) {
    // this must be a leaf node, proceed to entity query
    reRayQueryResult res;
    reFloat maxDistSq = RE_INFINITY;
    reEnt* resultEnt = nullptr;
    
    auto end = entities().qEnd();
    for (auto iter = entities().qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      if (q.queryID == query.ID) {
        continue;
      }
      re::queriesMade++;
      q.queryID = query.ID;
      if (q.ent->intersectsRay(query, res)) {
        if (res.distSq < maxDistSq) {
          resultEnt = q.ent;
          maxDistSq = res.distSq;
          result = res;
        }
      }
    }
    
    return resultEnt;
  } else {
    reRayQueryResult res[2];
    reEnt* ent[2] = { nullptr };
    
    for (reUInt i = 0; i < 2; i++) {
      // check if the hyperplane contains the ray
      if (query.dir.dot(_child[i]->_dir) > 0.0 || (query.origin - _child[i]->_anchor).dot(_child[i]->_dir) > 0.0) {
        ent[i] = _child[i]->queryWithRay(query, res[i]);
      }
    }
    
    if (res[0].distSq < res[1].distSq) {
      result = res[0];
      return ent[0];
    } else {
      result = res[1];
      return ent[1];
    }
    
    return nullptr;
  }
}

/**
 * Determines the optimal split plane for the node by sampling the contained
 * reEnt
 * 
 * @param anchor This field is set to the optimal anchor point
 * @param dir This field is set to the optimal split plane direction
 */

void reBSPTree::optimalSplit(reVector& anchor, reVector& dir) const {
  anchor.set(0.0, 0.0, 0.0);
  
  reVector dirs[RE_BSPTREE_GUESSES] = {
    _dir.cross(reVector::random()),
    _dir.cross(reVector::random()),
    _dir.cross(reVector::random())
  };
  
  reFloat vals[RE_BSPTREE_GUESSES] = { 0.0 };
  reUInt index = 0;
  
  if (size() > RE_BSPTREE_SAMPLE_SIZE) {
    reLinkedList<reEnt*> list = entities().sample(RE_BSPTREE_SAMPLE_SIZE);
    
    for (const reEnt* ent : list) {
      anchor += ent->center();
    }
    
    anchor /= RE_BSPTREE_SAMPLE_SIZE;
    
    reLinkedList<reEnt*> list2 = entities().sample(RE_BSPTREE_SAMPLE_SIZE);
    for (const reEnt* ent : list2) {
      for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
        vals[i] += dirs[i].dot(anchor - ent->center());
      }
    }
    
    for (reUInt i = 1; i < RE_BSPTREE_GUESSES; i++) {
      if (reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  } else {
    reUInt num = 0;
    reUInt n = reMin(RE_BSPTREE_SAMPLE_SIZE, size());
    
    auto end = entities().qEnd();
    for (auto iter = entities().qBegin(); iter != end; ++iter) {
      const reQueryable& q = *iter;
      anchor += q.ent->center();
      if (++num >= n) {
        break;
      }
    }
    anchor /= n;
    for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
      num = 0;
      auto end = entities().qEnd();
      for (auto iter = entities().qBegin(); iter != end; ++iter) {
        const reQueryable& q = *iter;
        vals[i] += dirs[i].dot(anchor - q.ent->center());
        if (++num >= n) {
          break;
        }
      }
      if (i > 0 && reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  }
  
  dir = dirs[index];
}

