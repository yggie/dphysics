#include "react/reWorld.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

#include <algorithm>

namespace {
  const reUInt MEM_SIZE = 1024*1024*1;
  char buffer[MEM_SIZE];

  reFreeListAllocator* flAlloc = nullptr;
  reProxyAllocator* proxy = nullptr;
};

reWorld::reWorld() : _bodies(), _broadPhase(nullptr), _updated(false) {
//  _broadPhase = re::alloc_new<reBSPTree>();
  if (re::globalAllocator == nullptr) {
    if (flAlloc == nullptr) {
      flAlloc = new reFreeListAllocator(MEM_SIZE, &buffer[0]);
    }
    if (proxy == nullptr) {
      proxy = new reProxyAllocator(flAlloc);
    }
    re::globalAllocator = proxy;
  }
}

reWorld::~reWorld() {
  clear();
  
//  re::alloc_delete<reBSPTree>((reBSPTree*)_broadPhase);
  if (proxy != nullptr) {
    delete proxy;
  }
  if (flAlloc != nullptr) {
    delete flAlloc;
  }
  re::globalAllocator = nullptr;
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
    re::alloc_delete((*it)->shape());
    re::alloc_delete(*it);
  }
  
  _bodies.clear();
  
//  _broadPhase->clear();
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
//  _broadPhase->add(entity);
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

reEnt* reWorld::shootRay(const reVector& from, const reVector& direction, reVector* intersect, reVector* normal) {
  ensureUpdate();
//  return _broadPhase->queryWithRay(from, direction, intersect, normal);
  reVector intersectPoint, intersectNormal;
  reVector closestPoint, closestNormal;
  reFloat maxDist = RE_INFINITY;
  reEnt* entContact = nullptr;
  
  for_each(_bodies.begin(), _bodies.end(), [&](reEnt* ent) {
    if (ent->intersectsRay(from, direction, &intersectPoint, &intersectNormal)) {
      reFloat dist = (from - intersectPoint).length();
      if (reIsLessThan(dist, maxDist)) {
        entContact = ent;
        maxDist = dist;
        closestPoint = intersectPoint;
        closestNormal = intersectNormal;
      }
    };
  });
  
  if (entContact == nullptr) {
    return nullptr;
  }
  
  if (intersect != nullptr) {
    *intersect = closestPoint;
  }
  
  if (normal != nullptr) {
    *normal = closestNormal;
  }
  
  return entContact;
}

void reWorld::ensureUpdate() {
  if (!_updated) {
//    for_each(_bodies.begin(), _bodies.end(), [](reEnt* ent) {
//      ent->update();
//    });
//    _broadPhase->update();
    _updated = true;
  }
}

