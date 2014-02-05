#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"
#include "react/Utilities/util_funcs.h"
#include "react/Collision/Shapes/reProxyShape.h"

reBSPNode::reBSPNode(reAllocator& allocator, reUInt depth) : _allocator(allocator), _markers(allocator), _children{nullptr}, _normal(), _center(), _depth(depth) {
  // do nothing
}

reBSPNode::~reBSPNode() {
  clear();
}

void reBSPNode::clear() {
  // clear children
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      _children[i]->clear();
      allocator().alloc_delete(_children[i]);
      _children[i] = nullptr;
    }
  }
  _markers.clear();
}

bool reBSPNode::remove(Marker& marker) {
  return _markers.remove(&marker);
}

/**
 * Called from the root or parent node to update its structure. If any reEnts
 * were removed, it is returned
 * 
 * @return A list of rejected _entities
 */

void reBSPNode::rebalanceNode(reTreeBalanceStrategy& strategy) {
  if (hasChildren()) {
    if (strategy.shouldMerge(*this)) {
      merge();
    } else {
      _children[0]->rebalanceNode(strategy);
      _children[1]->rebalanceNode(strategy);
    }
  } else {
    if (strategy.shouldSplit(*this)) {
      split(strategy);
    }
  }
}

/**
 * Updates the collision collection with collisions detected in the current
 * node
 * 
 * @param collisions The collision collection object
 */

void reBSPNode::updateContacts(reContactGraph& contacts, reEnt& entity) const {
  if (hasChildren()) {
    // propagate call to children
    switch (entity.fastPlaneIntersect(_normal, _center)) {
      case re::PlaneQuery::FRONT:
        _children[0]->updateContacts(contacts, entity);
        return;
      
      case re::PlaneQuery::INTERSECTS:
        // continue program
        break;
      
      case re::PlaneQuery::BEHIND:
        _children[1]->updateContacts(contacts, entity);
        return;
      
      default:
        RE_IMPOSSIBLE
    }
  }
  
  for (Marker* marker : _markers) {
    if (marker->entity.id() > entity.id()) {
      contacts.check(entity, marker->entity);
    } else if (marker->entity.id() < entity.id()) {
      contacts.check(marker->entity, entity);
    }
  }
}

bool reBSPNode::execute(reBSPTreeCallback callback) {
  if (hasChildren()) {
    if (_children[0]->execute(callback)) return true;
    if (_children[1]->execute(callback)) return true;
  }
  
  if (callback(*this)) return true;
  
  return false;
}

//reEnt* reBSPNode::queryWithRay(const reRayQuery& query, reRayQueryResult& result) const {
//  
//  if (!_entities.empty() && !isRoot()) {
//    // this must be a leaf node, proceed to entity query
//    reRayQueryResult res;
//    reFloat maxDistSq = RE_INFINITY;
//    reEnt* resultEnt = nullptr;
//    
//    auto end = _entities.qEnd();
//    for (auto iter = _entities.qBegin(); iter != end; ++iter) {
//      reQueryable& q = *iter;
//      if (q.queryID == query.ID) {
//        continue;
//      }
//      re::queriesMade++;
//      q.queryID = query.ID;
//      if (q.ent.intersectsRay(query, res)) {
//        if (res.distSq < maxDistSq) {
//          resultEnt = &q.ent;
//          maxDistSq = res.distSq;
//          result = res;
//        }
//      }
//    }
//    
//    return resultEnt;
//  } else {
//    reRayQueryResult res[2];
//    reEnt* ent[2] = { nullptr };
//    
//    for (reUInt i = 0; i < 2; i++) {
//      // check if the hyperplane contains the ray
//      if (re::dot(query.dir, _child[i]->dir) > 0.0 || re::dot(query.origin - _child[i]->anchor, _child[i]->dir) > 0.0) {
//        ent[i] = _child[i]->queryWithRay(query, res[i]);
//      }
//    }
//    
//    if (res[0].distSq < res[1].distSq) {
//      result = res[0];
//      return ent[0];
//    } else {
//      result = res[1];
//      return ent[1];
//    }
//    
//    return nullptr;
//  }
//}

/**
 * Called when the tree requires branching out
 */

void reBSPNode::split(reTreeBalanceStrategy& strategy) {
  SplitPlane splitPlane = strategy.computeSplitPlane(_normal, sample(8));
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _children[i] = _allocator.alloc_new<reBSPNode>(_allocator, _depth + 1);
    _children[i]->_normal = splitPlane.normal * ((i % 2 == 0) ? 1 : -1);
    _children[i]->_center = splitPlane.center;
  }
  
  for (Marker* marker : _markers) {
    marker->node = place(*marker);
  }
  
  _children[0]->rebalanceNode(strategy);
  _children[1]->rebalanceNode(strategy);
}

/**
 * Called when the _child nodes are to be merged with the parent
 */

void reBSPNode::merge() {
  for (reUInt i = 0; i < 2; i++) {
    _markers.append(_children[i]->_markers);
    _allocator.alloc_delete(_children[i]);
    _children[i] = nullptr;
  }
  
  // change node reference
  for (Marker* marker : _markers) {
    marker->node = this;
  }
}

const reLinkedList<reEnt*> reBSPNode::sample(reUInt num) const {
  reLinkedList<reEnt*> list(allocator());
  
  if (num < _markers.size()) {
    reUInt* indices = new reUInt[num];
    re::generateSortedUInts(indices, num, _markers.size());
    
    reUInt index = 0;
    reUInt counter = 0;
    for (Marker* marker : _markers) {
      if (counter++ == indices[index]) {
        list.add(&marker->entity);
        index++;
      }
      if (index == num) {
        break;
      }
    }
    
    delete[] indices;
  } else {
    for (Marker* marker : _markers) {
      list.add(&marker->entity);
    }
  }
  
  return list;
}

reBSPNode* reBSPNode::place(Marker& marker) {
  if (hasChildren()) {
    switch (marker.entity.fastPlaneIntersect(_normal, _center)) {
      case re::PlaneQuery::FRONT:
        return _children[0]->place(marker);
      
      case re::PlaneQuery::INTERSECTS:
        break;
      
      case re::PlaneQuery::BEHIND:
        return _children[1]->place(marker);
      
      default:
        RE_IMPOSSIBLE
    }
  }
  
  if (marker.node != this) {
    if (marker.node != nullptr) {
      marker.node->remove(marker);
    }
    marker.node = this;
    _markers.add(&marker);
  }
  
  return this;
}

/**
 * Passes the measure object through the structure recursively
 * 
 * @param The measure object, in which the data will be written on
 */

void reBSPNode::measureRecursive(reBPMeasure& m) const {
  m.children++;
  
  if (hasChildren()) {
    _children[0]->measureRecursive(m);
    _children[1]->measureRecursive(m);
  } else {
    m.leafs++;
    m.meanLeafDepth += _depth;
  }
  
  m.references += _markers.size();
}

reBSPTree::reBSPTree(reAllocator& allocator) : reBroadPhase(), reBSPNode(allocator, 0), _contacts(allocator), _strategy(), _masterMarkersList(allocator), _masterEntityList(allocator) {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
}

void reBSPTree::clear() {
  // clear all entities
  for (Marker* marker : _masterMarkersList) {
    RE_EXPECT(marker->entity.userdata == nullptr)
    if (marker->entity.shape()->type() == reShape::PROXY) {
      allocator().alloc_delete(
        ((reProxyShape*)marker->entity.shape())->shape()
      );
    }
    allocator().alloc_delete(marker->entity.shape());
    allocator().alloc_delete(&marker->entity);
    allocator().alloc_delete(marker);
  }
  
  // clear all broken references
  _masterMarkersList.clear();
  _masterEntityList.clear();
  
  reBSPNode::clear();
}

bool reBSPTree::add(reEnt& ent) {
  if (!_masterEntityList.contains(&ent)) {
    Marker* marker = allocator().alloc_new<Marker>(ent);
    _masterMarkersList.add(marker);
    _masterEntityList.add(&ent);
    place(*marker);
    return true;
  }
  
  return false;
}

bool reBSPTree::remove(reEnt& ent) {
  for (Marker* marker : _masterMarkersList) {
    if (marker->entity.id() == ent.id()) {
      if (marker->node != nullptr) {
        marker->node->remove(*marker);
      }
      
      _masterMarkersList.remove(marker);
      _masterEntityList.remove(&ent);
      allocator().alloc_delete(marker);
      return true;
    }
  }
  
  return false;
}

bool reBSPTree::contains(const reEnt& ent) const {
  // TODO very hacky way, must be a better method
  return _masterEntityList.contains(const_cast<reEnt*>(&ent));
}

void reBSPTree::rebalance(reTreeBalanceStrategy* strategy) {
  if (strategy == nullptr) {
    strategy = &_strategy;
  }
  
  reBSPNode::rebalanceNode(*strategy);
}

void reBSPTree::advance(reIntegrator& integrator, reFloat dt) {
  // advance each entity forward in time and relocates them on the tree
  for (Marker* marker : _markers) {
    marker->entity.advance(integrator, dt);
    place(*marker);
  }
  
  // update the contacts for each entity
  for (Marker* marker : _markers) {
    marker->node->updateContacts(_contacts, marker->entity);
  }
  
  // solves for the contact forces
  _contacts.solve();
  // advances the contact collection
  _contacts.advance();
}

void reBSPTree::addInteraction(reInteraction& action, reEnt& A, reEnt& B) {
  _contacts.addInteraction(action, A, B);
}

