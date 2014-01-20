#include "react/reWorld.h"

#include "react/Utilities/reIntegrator.h"
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

/**
 * Default constructor initializes the world with the default settings
 */

reWorld::reWorld() : _broadPhase(nullptr), _allocator(nullptr), _integrator(nullptr) {
  tmp = new SimpleAllocator();
  _allocator = new reProxyAllocator(tmp);
  _broadPhase = allocator().alloc_new<reBSPTree>(this);
  _integrator = allocator().alloc_new<reIntegrator>();
}

/**
 * Releases all resources used by the reWorld instance
 */

reWorld::~reWorld() {
  clear();
  
  allocator().alloc_delete(_broadPhase);
  allocator().alloc_delete(_integrator);
  
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
  
  _broadPhase->add(entity);
}

/**
 * Steps the reWorld forward in time by the step given
 * 
 * @param dt The time step to advance
 */

void reWorld::update(reFloat dt) {
  for (reEnt& ent : entities()) {
    ent.update(integrator(), dt);
  }
  _broadPhase->update();
}

/**
 * Returns a list of all reEnt contained in the reWorld
 * 
 * @return A list of reEnt
 */

reEntList& reWorld::entities() const {
  return _broadPhase->entities();
}

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& reWorld::newRigidBody(const reShape& shape) {
  reRigidBody* body = allocator().alloc_new<reRigidBody>(copyOf(shape));
  add(body);
  return *body;
}

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& reWorld::newRigidBody(const reShape& shape, const reTransform& transform) {
  reShape* base = copyOf(shape);
  reShape* newShape = allocator().alloc_new<reProxyShape>(base, &transform);
  reRigidBody* body = allocator().alloc_new<reRigidBody>(newShape);
  add(body);
  return *body;
}

/**
 * Performs a spatial query on the reBroadPhase with a ray defined by the input
 * data. The intersect point and normal is written to the optional parameters.
 * 
 * @param origin The origin of the ray
 * @param dir The ray direction
 * @param intersect If present, the point of intersection is written to it
 * @param normal If present, the intersection normal is written to it
 * @return The entity which was found
 */

reEnt* reWorld::queryWithRay(const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) {
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

/**
 * A convenient method to create copies of shapes
 */

reShape* reWorld::copyOf(const reShape& shape) const {
  switch (shape.type()) {
    case reShape::SPHERE:
      return allocator().alloc_new<reSphere>((const reSphere&)shape);
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::TRIANGLE:
      return allocator().alloc_new<reTriangle>((const reTriangle&)shape);
    
    case reShape::PROXY:
      RE_IMPOSSIBLE
      break;
  }
  
  RE_IMPOSSIBLE
  throw 0; // TODO proper exceptions
}


