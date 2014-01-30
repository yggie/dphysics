#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/reCollisionGraph.h"
#include "react/Collision/reTreeBalanceStrategy.h"
#include "react/Collision/Shapes/reProxyShape.h"

#include <cstdio>

reBSPTreeNode::reBSPTreeNode(const reWorld* world, reUInt depth, re::vec3 dir, re::vec3 anchor) : dir(dir), anchor(anchor), depth(depth), _world(*world), _entities(world), _child{nullptr} {
  // do nothing
}

reBSPTreeNode::~reBSPTreeNode() {
  clear();
}

void reBSPTreeNode::clear() {
  // clear children
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      _child[i]->clear();
      _world.allocator().alloc_delete(_child[i]);
      _child[i] = nullptr;
    }
  }
  
  // clear all entities
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

bool reBSPTreeNode::contains(const reEnt* ent) const {
  if (ent == nullptr) {
    RE_WARN("Attempted to query using null\n")
    return false;
  }
  
  return ent->intersectsHyperplane(anchor, dir);
}

reEnt* reBSPTreeNode::queryWithRay(const reRayQuery& query, reRayQueryResult& result) const {
  
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
      if (re::dot(query.dir, _child[i]->dir) > 0.0 || re::dot(query.origin - _child[i]->anchor, _child[i]->dir) > 0.0) {
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

void reBSPTreeNode::measure(reBPMeasure& m) const {
  if (isRoot()) {
    m.entities = _entities.size();
  } else {
    m.children++;
  }
  
  if (hasChildren()) {
    _child[0]->measure(m);
    _child[1]->measure(m);
  } else {
    m.leafs++;
    m.references += _entities.size();
    m.meanDepth += depth;
  }
  
  if (isRoot()) {
    m.meanDepth /= m.leafs;
  }
}

/**
 * Adds the queryable object to the structure
 * 
 * @param q The queryable object to add
 */

void reBSPTreeNode::add(reQueryable* q) {
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

/**
 * Removes the queryable object from the node
 * 
 * @param q The queryable object to remove
 */

void reBSPTreeNode::remove(reQueryable* q) {
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

/**
 * Called from the root or parent node to update its structure. If any reEnts
 * were removed, it is returned
 * 
 * @return A list of rejected _entities
 */

reEntList reBSPTreeNode::rebalanceNode(reTreeBalanceStrategy& strategy) {
  reEntList list(&_world);
  
  // updates the child nodes
  if (hasChildren()) {
    if (strategy.shouldMerge(*this)) {
      merge();
      return rebalanceNode(strategy);
    } else {
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->rebalanceNode(strategy));
        
        // moves entities between children if necessary
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
    // checks if entities are still contained
    list.append(trim());
    // split tree if necessary
    if (strategy.shouldSplit(*this)) {
      split(strategy);
    }
  }
  
  // returns a list of rejected entities
  return list;
}

/**
 * Checks if all entities are still contained and returns a list of rejected
 * entities.
 * 
 * @return A list of rejected entities
 */

reEntList reBSPTreeNode::trim() {
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

void reBSPTreeNode::split(reTreeBalanceStrategy& strategy) {
  re::vec3 splitAnchor, splitDir(dir);
  optimalSplit(splitAnchor, splitDir);
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = _world.allocator().alloc_new<reBSPTreeNode>(
      &_world,
      depth + 1,
      splitDir * ((i % 2 == 0) ? -1 : 1),
      splitAnchor
    );
    
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      if (_child[i]->contains(q.ent)) {
        _child[i]->add(&q);
      }
    }
    
    _child[i]->rebalanceNode(strategy);
  }
  
  if (!isRoot()) {
    _entities.clear();
  }
}

/**
 * Called when the _child nodes are to be merged with the parent
 */

void reBSPTreeNode::merge() {
  for (reUInt i = 0; i < 2; i++) {
    _entities.append(_child[i]->_entities);
    _world.allocator().alloc_delete(_child[i]);
    _child[i] = nullptr;
  }
  printf("[NODE] MERGE %d\n", depth);
}

/**
 * Determines the optimal split plane for the node by sampling the contained
 * reEnt
 * 
 * @param anchor This field is set to the optimal anchor point
 * @param dir This field is set to the optimal split plane direction
 */

void reBSPTreeNode::optimalSplit(re::vec3& anchor, re::vec3& dir) const {
  anchor.set(0.0, 0.0, 0.0);
  
  re::vec3 dirs[RE_BSPTREE_GUESSES] = {
    re::cross(dir, re::vec3::random()),
    re::cross(dir, re::vec3::random()),
    re::cross(dir, re::vec3::random())
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

reBSPTree::reBSPTree(const reWorld* world) : reBroadPhase(), reBSPTreeNode(world, 0, re::vec3(1.0, 0.0, 0.0), re::vec3(0.0, 0.0, 0.0)), _collisions(world), _strategy() {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
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

void reBSPTree::rebalance(reTreeBalanceStrategy* strategy) {
  if (strategy == nullptr) {
    strategy = &_strategy;
  }
  
  if (hasChildren()) {
    if (strategy->shouldMerge(*this)) {
      merge();
    } else {
      reEntList list(&_world);
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->rebalanceNode(*strategy));
        
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
    if (strategy->shouldSplit(*this)) {
      split(*strategy);
    }
  }
}

void reBSPTree::advance(reIntegrator& integrator, reFloat dt) {
  // advance each entity forward in time
  for (reEnt& e : _entities) {
    e.advance(integrator, dt);
  }
  // ensure queries will be up-to-date
  rebalance();
//  for (reBSPTree& leaf : leafs()) {
//    leaf.updateCollisions(_collisions);
//  }
  _collisions.solve();
}

