#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"

#include <cstdio>

void reBSPTree::show() {
    printf("[NODE]  at (%.1f, %.1f, %.1f) dir (%.1f, %.1f, %.1f) dep %d sz %d\n", _point[0], _point[1], _point[2], _dir[0], _dir[1], _dir[2], _depth, _size);
}

reBSPTree::reBSPTree() : _child{nullptr}, _size(0), _dir(), _point(0.0, 0.0, 0.0), _entities(), _depth(0) {
  // do nothing
}

reBSPTree::~reBSPTree() {
  clear();
}

void reBSPTree::clear() {
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      _child[i]->clear();
      re::alloc_delete(_child[i]);
      _child[i] = nullptr;
    }
  }
  _entities.clear();
}

bool reBSPTree::add(reEnt* ent) {
  if (ent == nullptr) {
    return false;
  }
  
  bool success = false;
  
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      if (_child[i]->contains(ent)) {
        success = _child[i]->add(ent) || success;
      }
    }
  } else {
    success = _entities.add(ent);
  }
  
  if (success) {
    _size++;
    return true;
  }
  
  return false;
}

bool reBSPTree::remove(reEnt* ent) {
  bool success = false;
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      success = (_child[i]->remove(ent) || success);
    }
  } else {
    success = _entities.remove(ent);
  }
  
  if (success) {
    _size--;
    return true;
  }
  
  return false;
}

void reBSPTree::update() {
  printf("[ROOT]  UPDATE %d\n", _depth);
  
  if (hasChildren()) {
    if (_child[0]->_size + _child[1]->_size < RE_BSPTREE_NODE_MIN_SIZE) {
      merge();
    } else {
      for (reUInt i = 0; i < 2; i++) {
        _entities.append(_child[i]->updateNode());

        for (reEnt* ent : _entities) {
          const reUInt j = (i + 1) % 2;
          if (_child[j]->contains(ent)) {
            _child[j]->add(ent);
            _entities.remove(ent);
          }
        }
      }
      
      RE_ASSERT(_entities.empty(), "reBSPTree list cannot be empty with children")
    }
  } else if (_size > RE_BSPTREE_NODE_MIN_SIZE) {
    _dir = reVector::random();
    split();
  }
  
  printf("[ROOT]  UPDATE COMPLETE %d\n", _depth);
  show();
}

reLinkedList<reEnt*> reBSPTree::entities() {
  reLinkedList<reEnt*> list(_entities);
  if (hasChildren()) {
    for (reUInt i = 0; i < 2; i++) {
      list.append(_child[i]->entities());
    }    
  }
  return list;
}

bool reBSPTree::contains(const reEnt* ent) const {
  if (ent == nullptr) {
    RE_LOG("Attempted to query for null ent")
    return false;
  }
  
  return ent->intersectsHyperplane(_point, _dir);
}

/**
 * Called from the root or parent node to update its structure. If any entities
 * were removed, it is returned
 * 
 * @return A list of rejected entities
 */

reLinkedList<reEnt*> reBSPTree::updateNode() {
  reLinkedList<reEnt*> list;
  printf("[NODE] UPDATE %d\n", _depth);
  
  if (hasChildren()) {
    if (_child[0]->_size + _child[1]->_size < RE_BSPTREE_NODE_MIN_SIZE) {
      merge();
      list.append(trim());
    } else {
      for (reUInt i = 0; i < 2; i++) {
        list.append(_child[i]->updateNode());
        
        for (reEnt* ent : list) {
          const reUInt j = (i + 1) % 2;
          if (_child[j]->contains(ent)) {
            _child[j]->add(ent);
            list.remove(ent);
          }
        }
      }
    }
  } else {
    list.append(trim());
    if (_size > RE_BSPTREE_NODE_MIN_SIZE && _depth < RE_BSPTREE_DEPTH_LIMIT) {
      split();
    }
  }
  
  printf("[NODE] UPDATE COMPLETE %d\n", _depth);
  show();
  
  return list;
}

/**
 * Called when the tree requires branching out
 */

void reBSPTree::split() {
  printf("[NODE] SPLIT %d\n", _depth);
  
  reVector ave;
  for (const reEnt* ent : _entities) {
    ave += ent->center();
  }
  ave /= _size;
  
  const reVector spawnDir = _dir.cross(reVector::random()).normalized();
  
  // setup each child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = re::alloc_new<reBSPTree>();
    _child[i]->_point = ave;
    _child[i]->_dir = spawnDir * ((i % 2 == 0) ? -1 : 1);
    _child[i]->_depth = _depth + 1;
    
    for (reEnt* ent : _entities) {
      if (_child[i]->contains(ent)) {
        _child[i]->add(ent);
      }
    }
    
    _child[i]->updateNode();
  }
  
  _entities.clear();
}

/**
 * Called when the child nodes are to be merged with the parent
 */

void reBSPTree::merge() {
  for (reUInt i = 0; i < 2; i++) {
    _entities.append(_child[i]->entities());
    re::alloc_delete<reBSPTree>(_child[i]);
    _child[i] = nullptr;
  }
  printf("[NODE] MERGE %d\n", _depth);
}

/**
 * Called when a node should verify if all entities are still contained. 
 * Returns a list of rejected entities
 * 
 * @return A list of rejected entities
 */

reLinkedList<reEnt*> reBSPTree::trim() {
  reLinkedList<reEnt*> rejected;
  for (reEnt* ent : _entities) {
    if (!contains(ent)) {
      remove(ent);
      rejected.add(ent);
    }
  }
  
  return rejected;
}

reEnt* reBSPTree::queryWithRay(const reRayQuery& query, reRayQueryResult& result) const {
  
  if (!_entities.empty()) {
    // this must be a leaf node, proceed to entity query
    reRayQueryResult res;
    reFloat maxDistSq = RE_INFINITY;
    reEnt* resultEnt = nullptr;
    
    for (reEnt* ent : _entities) {
      if (ent->intersectsRay(query, res)) {
        if (res.distSq < maxDistSq) {
          resultEnt = ent;
          maxDistSq = res.distSq;
          result = res;
        }
      }
    }
    
    return resultEnt;
  } else {
    reRayQueryResult res0, res1;
    reEnt* ent0 = _child[0]->queryWithRay(query, res0);
    reEnt* ent1 = _child[1]->queryWithRay(query, res1);
    
    if (ent1 != nullptr && ent0 != nullptr) {
      if (res0.distSq < res1.distSq) {
        result = res0;
        return ent0;
      } else {
        result = res1;
        return ent1;
      }
    } else if (ent1 != nullptr) {
      result = res1;
      return ent1;
    } else if (ent0 != nullptr) {
      result = res0;
      return ent0;
    }
    
    return nullptr;
  }
}

