#include "react/Collision/reBSPTree.h"

#include "react/math.h"
#include "react/Entities/reEnt.h"
#include "react/Memory/reAllocator.h"

#include <cstdio>

void reBSPTree::show() {
    printf("[NODE]  (%2d,%4d) at (%+.1f, %+.1f, %+.1f) dir (%+.1f, %+.1f, %+.1f)\n", _depth, _size, _point[0], _point[1], _point[2], _dir[0], _dir[1], _dir[2]);
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
  
  if (hasChildren()) {
    if (_child[0]->_size + _child[1]->_size < RE_BSPTREE_NODE_MIN_SIZE) {
      merge();
      return updateNode();
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
  
  if (!hasChildren()) {
    show();
  }
  
  return list;
}

/**
 * Called when the tree requires branching out
 */

void reBSPTree::split() {
  
  reVector splitPoint(0.0, 0.0, 0.0);
  reVector dirs[RE_BSPTREE_GUESSES] = {
    reVector::random(),
    reVector::random(),
    reVector::random()
  };
  reFloat vals[RE_BSPTREE_GUESSES] = { 0.0 };
  reUInt index = 0;
  
  if (_size > 2*RE_BSPTREE_SAMPLE_SIZE) {
    reUInt num = 0;
    for (const reEnt* ent : _entities) {
      if (num < RE_BSPTREE_SAMPLE_SIZE) {
        splitPoint += ent->center();
      } else if (num == RE_BSPTREE_SAMPLE_SIZE) {
        splitPoint /= RE_BSPTREE_SAMPLE_SIZE;
      } else {
        for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
          vals[i] += dirs[i].dot(splitPoint - ent->center());
        }
      }
      num++;
      if (num > 2*RE_BSPTREE_SAMPLE_SIZE) {
        break;
      }
    }
    for (reUInt i = 1; i < RE_BSPTREE_GUESSES; i++) {
      if (reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  } else {
    reUInt num = 0;
    reUInt n = reMin(RE_BSPTREE_SAMPLE_SIZE, _size);
    for (const reEnt* ent : _entities) {
      splitPoint += ent->center();
      if (++num >= n) {
        break;
      }
    }
    splitPoint /= n;
    for (reUInt i = 0; i < RE_BSPTREE_GUESSES; i++) {
      num = 0;
      for (const reEnt* ent : _entities) {
        vals[i] += dirs[i].dot(splitPoint - ent->center());
        if (++num >= n) {
          break;
        }
      }
      if (i > 0 && reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  }
  
  // setup each child node
  for (reUInt i = 0; i < 2; i++) {
    _child[i] = re::alloc_new<reBSPTree>();
    _child[i]->_point = splitPoint;
    _child[i]->_dir = dirs[index] * ((i % 2 == 0) ? -1 : 1);
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
    reRayQueryResult res[2];
    reEnt* ent[2] = { nullptr };
    
    for (reUInt i = 0; i < 2; i++) {
      // check if the hyperplane contains the ray
      if (query.dir.dot(_child[i]->_dir) > 0.0 || (query.origin - _child[i]->_point).dot(_child[i]->_dir) > 0.0) {
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

