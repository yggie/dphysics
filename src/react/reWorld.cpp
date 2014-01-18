#include "react/reWorld.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

#include <algorithm>

// strictly for debugging
namespace {
  struct SimpleAllocator : public reBaseAllocator {
  public:
    SimpleAllocator() : n(0) { }
    
    void* alloc(u32 size, u8) { n++; return malloc(size); }
    void dealloc(void* p) { n--; free(p); }
    
    u32 used() const override { return 0; }
    u32 numAllocs() const override { return n; }
    u32 size() const override { return 0; }
    void* ptr() const override { return nullptr; }
  
  private:
    reUInt n;
  };
  
  SimpleAllocator* tmp;
}

reWorld::reWorld() : _broadPhase(nullptr), _allocator(nullptr), _updated(false) {
  tmp = new SimpleAllocator();
  _allocator = new reProxyAllocator(tmp);
  _broadPhase = allocator().alloc_new<reBSPTree>();
}

reWorld::~reWorld() {
  clear();
  
  allocator().alloc_delete<reBSPTree>((reBSPTree*)_broadPhase);
  
  if (_allocator != nullptr) {
    delete _allocator;
    delete tmp;
  }
}

/**
 * Removes all entities in the reWorld
 */

void reWorld::clear() {
  _broadPhase->clear();
  _updated = true;
}

reRigidBody& reWorld::newRigidBody() {
  reRigidBody* body = allocator().alloc_new<reRigidBody>(this);
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

void reWorld::update(reFloat dt) {
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

/**
 * A convenient method to create copies of shapes
 */

reShape& reWorld::copyOf(const reShape& shape) const {
  switch (shape.type()) {
    case reShape::SPHERE:
      return *allocator().alloc_new<reSphere>((const reSphere&)shape);
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::TRIANGLE:
      return *allocator().alloc_new<reTriangle>((const reTriangle&)shape);
    
    case reShape::PROXY:
      {
        // TODO fix very bad bug with distorted shape unable to allocate shapes
        const reProxyShape& orig = (const reProxyShape&)shape;
        reProxyShape* copy = allocator().alloc_new<reProxyShape>(this);
        copy->setShape(orig.shape());
        copy->setTransform(orig.transform());
        return *copy;
      }
  }
  
  RE_IMPOSSIBLE
  return *allocator().alloc_new<reSphere>(1.0);
}


