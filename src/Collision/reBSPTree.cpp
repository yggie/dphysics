#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/Entity.h"
#include "react/Memory/reAllocator.h"
#include "react/Utilities/util_funcs.h"
#include "react/Collision/Shapes/ShapeProxy.h"

reBSPNode::reBSPNode(reAllocator& allocator, reUInt depth) : _allocator(allocator), _markers(allocator), _children{nullptr}, _splitPlane(re::vec3(1.0, 0.0, 0.0), 0.0), _depth(depth) {
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
 * Called from the root or parent node to update its structure. If any re::Entitys
 * were removed, it is returned
 * 
 * @return A list of rejected _entities
 */

void reBSPNode::rebalanceNode(re::Strategy& strategy) {
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

void reBSPNode::updateContacts(re::ContactGraph& contacts, re::Entity& entity) const {
  if (hasChildren()) {
    // propagate call to children
    switch (entity.relativeToPlane(_splitPlane)) {
      case re::FRONT:
        _children[0]->updateContacts(contacts, entity);
        return;
      
      case re::INTERSECT:
        // continue program
        break;
      
      case re::BACK:
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

#include "react/debug.h"

void reBSPNode::queryWithRay(const re::Ray& ray, re::RayQuery& result) const {
  re::RayQuery res;
  for (Marker* marker : _markers) {
    re::queriesMade++;
    if (marker->entity.intersects(ray, res)) {
      if (res.depth < result.depth) {
        result.point = res.point;
        result.normal = res.normal;
        result.depth = res.depth;
        result.entity = &marker->entity;
      }
    }
  }

  if (hasChildren()) {
    const reFloat a = re::dot(ray.dir(), _splitPlane.normal());
    const reFloat b = re::dot(_splitPlane.normal(), ray.origin()) - _splitPlane.offset();
    if (a > RE_FP_TOLERANCE && b > RE_FP_TOLERANCE) {
      _children[0]->queryWithRay(ray, result);
    } else if (a < RE_FP_TOLERANCE && b < RE_FP_TOLERANCE) {
      _children[1]->queryWithRay(ray, result);
    } else {
      _children[0]->queryWithRay(ray, result);
      _children[1]->queryWithRay(ray, result);
    }
  }
}

/**
 * Called when the tree requires branching out
 */

void reBSPNode::split(re::Strategy& strategy) {
  // use parent split normal to determine optimal split plane
  _splitPlane = strategy.computeSplitPlane(_splitPlane.normal(), sample(8));
  
  // setup each _child node
  for (reUInt i = 0; i < 2; i++) {
    _children[i] = _allocator.alloc_new<reBSPNode>(_allocator, _depth + 1);
    // keep a reference to the parent split plane for later
    _children[i]->_splitPlane = _splitPlane;
  }
  
  auto end = _markers.end();
  for (auto it = _markers.begin(); it != end;) {
    Marker* marker = *it;
    ++it;
    place(*marker);
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

const reLinkedList<re::Entity*> reBSPNode::sample(reUInt num) const {
  reLinkedList<re::Entity*> list(allocator());
  
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
    switch (marker.entity.relativeToPlane(_splitPlane)) {
      case re::FRONT:
        return _children[0]->place(marker);
      
      case re::INTERSECT:
        break;
      
      case re::BACK:
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

reBSPTree::reBSPTree(reAllocator& allocator) : reBroadPhase(), reBSPNode(allocator, 0), _contacts(allocator), _strategy(), _allMarkers(allocator), _masterEntityList(allocator) {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
}

void reBSPTree::clear() {
  // clear all entities
  for (Marker* marker : _allMarkers) {
    RE_EXPECT(marker->entity.userdata == nullptr)
    if (marker->entity.shape().type() == reShape::PROXY) {
      allocator().alloc_delete(
        ((re::ShapeProxy&)marker->entity.shape()).shape()
      );
    }
    allocator().alloc_delete(&marker->entity.shape());
    allocator().alloc_delete(&marker->entity);
    allocator().alloc_delete(marker);
  }
  
  // clear all broken references
  _allMarkers.clear();
  _masterEntityList.clear();
  
  reBSPNode::clear();
}

bool reBSPTree::add(re::Entity& ent) {
  if (!_masterEntityList.contains(&ent)) {
    Marker* marker = allocator().alloc_new<Marker>(ent);
    _allMarkers.add(marker);
    _masterEntityList.add(&ent);
    place(*marker);
    return true;
  }
  
  return false;
}

bool reBSPTree::remove(re::Entity& ent) {
  for (Marker* marker : _allMarkers) {
    if (marker->entity.id() == ent.id()) {
      if (marker->node != nullptr) {
        marker->node->remove(*marker);
      }
      
      _allMarkers.remove(marker);
      _masterEntityList.remove(&ent);
      allocator().alloc_delete(marker);
      return true;
    }
  }
  
  return false;
}

bool reBSPTree::contains(const re::Entity& ent) const {
  // TODO very hacky way, must be a better method
  return _masterEntityList.contains(const_cast<re::Entity*>(&ent));
}

void reBSPTree::rebalance(re::Strategy* strategy) {
  if (strategy == nullptr) {
    strategy = &_strategy;
  }
  
  reBSPNode::rebalanceNode(*strategy);
}

void reBSPTree::advance(re::Integrator& integrator, reFloat dt) {
  // advance each entity forward in time and relocates them on the tree
  auto end = _allMarkers.end();
  for (auto it = _allMarkers.begin(); it != end;) {
    Marker* marker = *it;
    marker->entity.advance(integrator, dt);
    ++it;
    place(*marker);
  }
  
  // update the contacts for each entity
  for (Marker* marker : _allMarkers) {
    marker->node->updateContacts(_contacts, marker->entity);
  }
  
  // solves for the contact forces
  _contacts.solve();
  // advances the contact collection
  _contacts.advance();
}

void reBSPTree::addInteraction(reInteraction& action, re::Entity& A, re::Entity& B) {
  _contacts.addInteraction(action, A, B);
}

