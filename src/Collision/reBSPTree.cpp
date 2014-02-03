#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/Shapes/reProxyShape.h"

reBSPTreeNode::reBSPTreeNode(reAllocator& allocator, reUInt depth, re::vec3 dir, re::vec3 anchor) : dir(dir), anchor(anchor), depth(depth), _allocator(allocator), _entities(allocator), _child{nullptr} {
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
      _allocator.alloc_delete(_child[i]);
      _child[i] = nullptr;
    }
  }
  
  // clear all entities
  if (isRoot()) {
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      RE_EXPECT(q.ent.userdata == nullptr)
      if (q.ent.shape()->type() == reShape::PROXY) {
        _allocator.alloc_delete(((reProxyShape*)q.ent.shape())->shape());
      }
      _allocator.alloc_delete(q.ent.shape());
      _allocator.alloc_delete(&q.ent);
      _allocator.alloc_delete(&q);
    }
  }
  _entities.clear();
}

bool reBSPTreeNode::intersects(const reEnt& ent) const {
  return ent.intersectsHyperplane(anchor, dir);
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
      if (q.ent.intersectsRay(query, res)) {
        if (res.distSq < maxDistSq) {
          resultEnt = &q.ent;
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

reBPMeasure reBSPTreeNode::measure() const {
  reBPMeasure m;
  measureRecursive(m);
  return m;
}

/**
 * Adds the queryable object to the structure
 * 
 * @param q The queryable object to add
 */

bool reBSPTreeNode::add(reQueryable& q) {
  bool success = false;
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->intersects(q.ent)) {
        success = _child[i]->add(q) || success;
      }
    }
  } else {
    success = _entities.add(q);
  }
  return success;
}

/**
 * Removes the queryable object from the node
 * 
 * @param q The queryable object to remove
 */

bool reBSPTreeNode::remove(reQueryable& q) {
  bool found = false;
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->intersects(q.ent)) {
        found = _child[i]->remove(q) || found;
      }
    }
  } else {
    found = _entities.remove(q);
  }
  
  if (found) {
    _allocator.alloc_delete(&q);
  }
  
  return found;
}

/**
 * Called from the root or parent node to update its structure. If any reEnts
 * were removed, it is returned
 * 
 * @return A list of rejected _entities
 */

reEntList reBSPTreeNode::rebalanceNode(reTreeBalanceStrategy& strategy) {
  reEntList list(_allocator);
  
  // updates the child nodes
  if (hasChildren()) {
    if (strategy.shouldMerge(*this)) {
      merge();
      return rebalanceNode(strategy);
    } else {
      for (reUInt i = 0; i < 2; i++) {
        reEntList el = _child[i]->rebalanceNode(strategy);
        printf("LOOKING@%d\n", _child[i]->depth);
        for (reEnt& ent : el) {
          RE_ASSERT(&ent != nullptr)
        }
        list.append(el);
        
        // moves entities between children if necessary
        auto end = list.qEnd();
        for (auto iter = list.qBegin(); iter != end; ++iter) {
          reQueryable& q = *iter;
          const reUInt j = (i + 1) % 2;
          if (_child[j]->intersects(q.ent)) {
            _child[j]->add(q);
            list.remove(q);
          }
        }
      }
    }
  } else {
    // checks if entities are still contained
    printf("APP@%d\n", depth);
    list.append(trim());
    // split tree if necessary
    if (strategy.shouldSplit(*this)) {
      split(strategy);
    }
  }
  
  printf("REBALANCE@%d=%d\n", depth, list.size());
  // returns a list of rejected entities
  return list;
}

/**
 * Updates the collision collection with collisions detected in the current
 * node
 * 
 * @param collisions The collision collection object
 */

void reBSPTreeNode::updateContacts(reContactGraph& collisions) const {
  if (hasChildren()) {
    // propagate call to children
    _child[0]->updateContacts(collisions);
    _child[1]->updateContacts(collisions);
  } else {
    auto end = _entities.end();
    // loop over all entities
    for (auto it = _entities.begin(); it != end; ++it) {
      reEnt& ie = *it;
      for (auto jt = it + 1; jt != end; ++jt) {
        reEnt& je = *jt;
        // checks the interactions between these entities
        collisions.check(ie, je);
      }
    }
  }
}

/**
 * Checks if all entities are still contained and returns a list of rejected
 * entities.
 * 
 * @return A list of rejected entities
 */

reEntList reBSPTreeNode::trim() {
  reEntList rejected(_allocator);
  auto end = _entities.qEnd();
  for (auto iter = _entities.qBegin(); iter != end; ++iter) {
    reQueryable& q = *iter;
    if (!intersects(q.ent)) {
      rejected.add(q);
    }
  }
  
  end = rejected.qEnd();
  for (auto iter = rejected.qBegin(); iter != end; ++iter) {
    remove(*iter);
  }
  
  printf("TRIM@%d\n", depth);
  return rejected;
}

/**
 * Called when the tree requires branching out
 */

void reBSPTreeNode::split(reTreeBalanceStrategy& strategy) {
  re::vec3 splitAnchor, splitDir(dir);
  strategy.computeSplitPlane(dir, _entities, splitAnchor, splitDir);
//  optimalSplit(splitAnchor, splitDir);
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = _allocator.alloc_new<reBSPTreeNode>(
      _allocator,
      depth + 1,
      splitDir * ((i % 2 == 0) ? -1 : 1),
      splitAnchor
    );
    
    auto end = _entities.qEnd();
    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
      reQueryable& q = *iter;
      if (_child[i]->intersects(q.ent)) {
        _child[i]->add(q);
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
    _allocator.alloc_delete(_child[i]);
    _child[i] = nullptr;
  }
  printf("[NODE] MERGE %d\n", depth);
}

/**
 * Passes the measure object through the structure recursively
 * 
 * @param The measure object, in which the data will be written on
 */

void reBSPTreeNode::measureRecursive(reBPMeasure& m) const {
  if (isRoot()) {
    m.entities = _entities.size();
  } else {
    m.children++;
  }
  
  if (hasChildren()) {
    _child[0]->measureRecursive(m);
    _child[1]->measureRecursive(m);
  } else {
    m.leafs++;
    m.references += _entities.size();
    m.meanDepth += depth;
  }
  
  if (isRoot()) {
    m.meanDepth /= m.leafs;
  }
}

bool reBSPTreeNode::execute(reBSPTreeCallback callback) {
  if (hasChildren()) {
    if (_child[0]->execute(callback)) return true;
    if (_child[1]->execute(callback)) return true;
  }
  
  if (callback(*this)) return true;
  
  return false;
}

reBSPTree::reBSPTree(reAllocator& allocator) : reBroadPhase(), reBSPTreeNode(allocator, 0, re::vec3(1.0, 0.0, 0.0), re::vec3(0.0, 0.0, 0.0)), _contacts(allocator), _strategy() {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
}

bool reBSPTree::add(reEnt& ent) {
  reQueryable* q = _allocator.alloc_new<reQueryable>(ent);
  if (_entities.add(*q)) {
    if (hasChildren()) {
      for (reUInt i = 0; i < 2; i++) {
        if (_child[i]->intersects(ent)) {
          _child[i]->add(*q);
        }
      }
    }
    return true;
  } else {
    _allocator.alloc_delete(q);
    return false;
  }
}

bool reBSPTree::remove(reEnt& ent) {
  auto end = _entities.qEnd();
  for (auto iter = _entities.qBegin(); iter != end; ++iter) {
    reQueryable& q = *iter;
    if (q.ent.id() == ent.id()) {
      if (hasChildren()) {
        for (reUInt i = 0; i < 2; i++) {
          if (_child[i]->intersects(ent)) {
            _child[i]->remove(q);
          }
        }
      }
      _entities.remove(q);
      _allocator.alloc_delete(&q);
      return true;
    }
  }
  
  return false;
}

bool reBSPTree::contains(const reEnt& ent) const {
  return _entities.contains(ent);
}

void reBSPTree::rebalance(reTreeBalanceStrategy* strategy) {
  if (strategy == nullptr) {
    strategy = &_strategy;
  }
  
  if (hasChildren()) {
    if (strategy->shouldMerge(*this)) {
      merge();
    } else {
      reEntList list(_allocator);
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->rebalanceNode(*strategy));
        
        auto end = list.qEnd();
        for (auto iter = list.qBegin(); iter != end; ++iter) {
          reQueryable& q = *iter;
          const reUInt j = (i + 1) % 2;
          if (_child[j]->intersects(q.ent)) {
            _child[j]->add(q);
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
  // updates the contacts
  updateContacts(_contacts);
  // solves for the contact forces
  _contacts.solve();
  // advances the contact collection
  _contacts.advance();
}

void reBSPTree::addInteraction(reInteraction& action, reEnt& A, reEnt& B) {
  _contacts.addInteraction(action, A, B);
}

