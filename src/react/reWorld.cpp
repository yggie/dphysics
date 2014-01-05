#include "react/reWorld.h"

#include "react/memory/reProxyAllocator.h"
#include "react/memory/reFreeListAllocator.h"

#include "react/reRigidBody.h"
#include "react/reSphere.h"
#include "react/reTriangle.h"
#include "react/reDistortedShape.h"

#include <algorithm>

const int MEM_ALLOC = 1024*1024*1;
//const int MEM_ALLOC = 350;

char buffer[MEM_ALLOC];

reWorld::reWorld() : _bodies() {
  reFreeListAllocator* tmp = new reFreeListAllocator(MEM_ALLOC, &buffer[0]);
  _allocator = new reProxyAllocator(tmp);
}

reWorld::~reWorld() {
  clear();
  
  RE_ASSERT(_allocator != nullptr, "No allocator was defined for the world object")
  if (_allocator != nullptr) {
    delete _allocator;
    _allocator = nullptr;
  }
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
    remove((*it)->shape());
    _allocator->alloc_delete<reEnt>(*it);
  }
  
  _bodies.clear();
}

reRigidBody& reWorld::newRigidBody() {
  reRigidBody* body = _allocator->alloc_new<reRigidBody>();
  add(*body);
  return *body;
}

reShape& reWorld::copyOf(const reShape& shape) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return *_allocator->alloc_new<reSphere>((const reSphere&)shape);
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::TRIANGLE:
      return *_allocator->alloc_new<reTriangle>((const reTriangle&)shape);
    
    case reShape::DISTORTED:
      return (*_allocator->alloc_new<reDistortedShape>((const reDistortedShape&)shape)).withWorld(*this);
  }
  
  RE_IMPOSSIBLE
  
  return *_allocator->alloc_new<reSphere>(1.0);
}

/**
 * Registers the input reEnt to the reWorld's engine.
 * 
 * @param entity The entity to attach
 */

void reWorld::add(reEnt& entity) {
  
  RE_ASSERT_WARN(entity.world() == nullptr, "The entity was already registered to another world")
  
  entity.setWorld(this);
  switch (entity.type()) {
    case reEnt::RIGID:
      _bodies.push_back((reRigidBody*)(&entity));
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

void reWorld::remove(reShape* shape) {
  if (shape == nullptr) {
    RE_LOG("Attempted to delete NULL shape")
    return;
  }
  
  switch (shape->type()) {
    case reShape::SPHERE:
      _allocator->alloc_delete<reSphere>((reSphere*)shape);
      return;
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::TRIANGLE:
      _allocator->alloc_delete<reTriangle>((reTriangle*)shape);
      return;
    
    case reShape::DISTORTED:
      _allocator->alloc_delete<reDistortedShape>((reDistortedShape*)shape);
      return;
  }
  
  RE_IMPOSSIBLE
}

void reWorld::step(reFloat dt) {
  // do nothing
  dt += 1;
}

reEnt* reWorld::shootRay(const reVector& from, const reVector& direction, reVector* intersect, reVector* normal) {
  reVector intersectPoint, intersectNormal;
  reVector closestPoint, closestNormal;
  reFloat maxDist = RE_INFINITY;
  reEnt* entContact = nullptr;
  
  for_each(_bodies.begin(), _bodies.end(), [&](reEnt* ent) {
    if (ent->rayIntersect(from, direction, &intersectPoint, &intersectNormal)) {
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

