#include "react/reWorld.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

#include <algorithm>

reWorld::reWorld() : _bodies(), _broadPhase(nullptr), _updated(false) {
  _broadPhase = re::alloc_new<reBSPTree>();
}

reWorld::~reWorld() {
  clear();
  
  re::alloc_delete<reBSPTree>((reBSPTree*)_broadPhase);
}

/**
 * Removes all entities in the reWorld
 */

void reWorld::clear() {
  std::vector<reRigidBody*>::iterator it;
  std::vector<reRigidBody*>::iterator itEnd = _bodies.end();
  
  RE_LOG("No. of bodies = %ld", _bodies.size())
  
//  reProxyAllocator* proxy = (reProxyAllocator*)_allocator;
  for (it = _bodies.begin(); it != itEnd; it++) {
//    proxy->show();
    RE_ASSERT_WARN((*it)->userdata == nullptr, "This body has a non-null userdata pointer!")
    re::alloc_delete((*it)->shape());
    re::alloc_delete(*it);
  }
  
  _bodies.clear();
  
  _broadPhase->clear();
  _updated = true;
}

reRigidBody& reWorld::newRigidBody() {
  reRigidBody* body = re::alloc_new<reRigidBody>();
  add(body);
  return *body;
}

/**
 * Registers the input reEnt to the reWorld's engine.
 * 
 * @param entity The entity to attach
 */

void reWorld::add(reEnt* entity) {
  if (entity == nullptr) {
    RE_LOG("Attempted to add null entity")
    return;
  }
  
  _updated = false;
  _broadPhase->add(entity);
  switch (entity->type()) {
    case reEnt::RIGID:
      _bodies.push_back((reRigidBody*)(entity));
      break;
    
    case reEnt::STATIC:
      RE_NOT_IMPLEMENTED
      break;
    
    case reEnt::FIELD:
      RE_NOT_IMPLEMENTED
      break;
    
    case reEnt::PARTICLE:
      RE_NOT_IMPLEMENTED
      break;
    
    default:
      RE_IMPOSSIBLE
      break;
  }
}

void reWorld::step(reFloat dt) {
  // do nothing
  dt += 1;
}

reEnt* reWorld::queryWithRay(const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) {
  ensureUpdate();
  
  reRayQuery query;
  query.origin = origin;
  query.dir = dir;
  reRayQueryResult result;
  
  reEnt* ent = _broadPhase->queryWithRay(query, result);
  
  if (ent != nullptr) {
    if (intersect != nullptr) {
      *intersect = result.intersect;
    }
    if (normal != nullptr) {
      *normal = result.normal;
    }
    return ent;
  }
  
  return nullptr;

//  reRayQuery query;
//  query.origin = origin;
//  query.dir = dir;
//  reRayQueryResult res, result;
//  
//  reFloat maxDistSq = RE_INFINITY;
//  reEnt* entContact = nullptr;

//  for (auto ent : _bodies) {
//    if (ent->intersectsRay(query, res)) {
////      const reFloat distSq = (origin - intersectPoint).lengthSq();
//      if (res.distSq < maxDistSq) {
//        entContact = ent;
//        maxDistSq = res.distSq;
//        result = res;
//      }
//    };
//  };
//  
//  if (entContact == nullptr) {
//    return nullptr;
//  }
//  
//  if (intersect != nullptr) {
//    *intersect = res.intersect;
//  }
//  
//  if (normal != nullptr) {
//    *normal = res.normal;
//  }
//  
//  return entContact;
}

void reWorld::ensureUpdate() {
  if (!_updated) {
//    for_each(_bodies.begin(), _bodies.end(), [](reEnt* ent) {
//      ent->update();
//    });
    _broadPhase->update();
    _updated = true;
  }
}

