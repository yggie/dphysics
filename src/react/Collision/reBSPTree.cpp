#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/reCollisionGraph.h"
#include "react/Collision/reTreeBalanceStrategy.h"
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
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
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
  _entities.clear();
}

bool reBSPTree::add(reEnt* ent) {
  if (ent == nullptr) {
    return false;
  }
  
  reQueryable* q = _world.allocator().alloc_new<reQueryable>(ent);
  if (_entities.add(q)) {
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
  auto end = _entities.qEnd();
  for (auto iter = _entities.qBegin(); iter != end; ++iter) {
    reQueryable& q = *iter;
    if (q.ent->id() == ent->id()) {
      if (hasChildren()) {
        for (reUInt i = 0; i < 2; i++) {
          if (_child[i]->contains(ent)) {
            _child[i]->remove(&q);
          }
        }
      }
      _entities.remove(&q);
      _world.allocator().alloc_delete(&q);
      return true;
    }
  }
  
  return false;
}

void reBSPTree::update() {
  if (hasChildren()) {
    if (_strategy.shouldMerge(*this)) {
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
    if (_strategy.shouldSplit(*this)) {
      split();
    }
  }
  
//  printf("[ROOT] (%3d, %3d) (%+.1f, %+.1f, %+.1f) (%+.1f, %+.1f, %+.1f)\n", _depth, size(), _dir[0], _dir[1], _dir[2], _anchor[0], _anchor[1], _anchor[2]);
}

void reBSPTree::advance(reIntegrator& integrator, reFloat dt) {
  // advance each entity forward in time
  for (reEnt& e : _entities) {
    e.advance(integrator, dt);
  }
  // ensure queries will be up-to-date
  update();
//  for (reBSPTree& leaf : leafs()) {
//    leaf.updateCollisions(_collisions);
//  }
  _collisions.solve();
}

void reBSPTree::add(reQueryable* q) {
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->contains(q->ent)) {
        _child[i]->add(q);
      }
    }
  } else {
    _entities.add(q);
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
    _entities.remove(q);
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
 * @return A list of rejected _entities
 */

reEntList reBSPTree::updateNode() {
  reEntList list(&_world);
  
  if (hasChildren()) {
    if (_strategy.shouldMerge(*this)) {
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
    if (_strategy.shouldSplit(*this)) {
      split();
    }
  }
  
//  if (!hasChildren()) {
//    printf("[NODE] (%3d, %3d) (%+.1f, %+.1f, %+.1f) (%+.1f, %+.1f, %+.1f)\n", _depth, size(), _dir[0], _dir[1], _dir[2], _anchor[0], _anchor[1], _anchor[2]);
//  }
  
  return list;
}

/**
 * Called when a node should verify if all _entities are still contained. 
 * Returns a list of rejected _entities
 * 
 * @return A list of rejected _entities
 */

reEntList reBSPTree::trim() {
  reEntList rejected(&_world);
  auto end = _entities.qEnd();
  for (auto iter = _entities.qBegin(); iter != end; ++iter) {
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
  re::vec3 splitAnchor, splitDir;
  optimalSplit(splitAnchor, splitDir);
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = _world.allocator().alloc_new<reBSPTree>(&_world, _depth + 1);
    _child[i]->_anchor = splitAnchor;
    _child[i]->_dir = splitDir * ((i % 2 == 0) ? -1 : 1);
    
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      if (_child[i]->contains(q.ent)) {
        _child[i]->add(&q);
      }
    }
    
    _child[i]->updateNode();
  }
  
  if (!isRoot()) {
    _entities.clear();
  }
}

/**
 * Called when the _child nodes are to be merged with the parent
 */

void reBSPTree::merge() {
  for (reUInt i = 0; i < 2; i++) {
    _entities.append(_child[i]->_entities);
    _world.allocator().alloc_delete<reBSPTree>(_child[i]);
    _child[i] = nullptr;
  }
  printf("[NODE] MERGE %d\n", _depth);
}

void reBSPTree::measure(reBPMeasure& measure) const {
  if (isRoot()) {
    measure.entities = _entities.size();
  } else {
    measure.children++;
  }
  
  if (hasChildren()) {
    _child[0]->measure(measure);
    _child[1]->measure(measure);
  } else {
    measure.leafs++;
    measure.references += _entities.size();
    measure.meanDepth += _depth;
  }
  
  if (isRoot()) {
    measure.meanDepth /= measure.leafs;
  }
}

reEnt* reBSPTree::queryWithRay(const reRayQuery& query, reRayQueryResult& result) const {
  
  if (!_entities.empty() && !isRoot()) {
    // this must be a leaf node, proceed to entity query
    reRayQueryResult res;
    reFloat maxDistSq = RE_INFINITY;
    reEnt* resultEnt = nullptr;
    
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
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
      if (re::dot(query.dir, _child[i]->_dir) > 0.0 || re::dot(query.origin - _child[i]->_anchor, _child[i]->_dir) > 0.0) {
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

void reBSPTree::optimalSplit(re::vec3& anchor, re::vec3& dir) const {
  anchor.set(0.0, 0.0, 0.0);
  
  re::vec3 dirs[RE_BSPTREE_GUESSES] = {
    re::cross(_dir, re::vec3::random()),
    re::cross(_dir, re::vec3::random()),
    re::cross(_dir, re::vec3::random())
  };
  
  reFloat vals[RE_BSPTREE_GUESSES] = { 0.0 };
  reUInt index = 0;
  
  if (size() > RE_BSPTREE_SAMPLE_SIZE) {
    reLinkedList<reEnt*> list = _entities.sample(RE_BSPTREE_SAMPLE_SIZE);
    
    for (const reEnt* ent : list) {
      anchor += ent->center();
    }
    
    anchor /= RE_BSPTREE_SAMPLE_SIZE;
    
    reLinkedList<reEnt*> list2 = _entities.sample(RE_BSPTREE_SAMPLE_SIZE);
    for (const reEnt* ent : list2) {
      for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
        vals[i] += re::dot(dirs[i], anchor - ent->center());
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
    
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
      const reQueryable& q = *iter;
      anchor += q.ent->center();
      if (++num >= n) {
        break;
      }
    }
    anchor /= n;
    for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
      num = 0;
      auto end = _entities.qEnd();
      for (auto iter = _entities.qBegin(); iter != end; ++iter) {
        const reQueryable& q = *iter;
        vals[i] += re::dot(dirs[i], anchor - q.ent->center());
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

