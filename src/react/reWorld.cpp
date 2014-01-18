#include "react/reWorld.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

#include <algorithm>

reWorld::reWorld() : _broadPhase(nullptr), _updated(false) {
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
  _broadPhase->clear();
  _updated = true;
}

reRigidBody& reWorld::newRigidBody() {
  reRigidBody* body = re::alloc_new<reRigidBody>();
  add(body);
  return *body;
}

void reWorld::forEachEntDo(void(*func)(reEnt* ent)) {
  _broadPhase->forEachEntDo(func);
}

/**
 * Registers the input reEnt to the reWorld's engine.
 * 
 * @param entity The entity to attach
 */

void reWorld::add(reEnt* entity) {
  if (entity == nullptr) {
    RE_WARN("Attempted to add null entity to world\n")
    return;
  }
  
  _updated = false;
  _broadPhase->add(entity);
}

void reWorld::step(reFloat dt) {
  // do nothing
//  _broadPhase->forEachEntDo([](reEnt* ent) {
//    ent->step(dt);
//  });
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
}

void reWorld::ensureUpdate() {
  if (!_updated) {
    _broadPhase->update();
    _updated = true;
  }
}

