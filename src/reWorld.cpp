#include "react/reWorld.h"

#include "react/Math/Integrator.h"
#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/Dynamics/reGravAction.h"

#include "react/Memory/reFreeListAllocator.h"
#include "react/Memory/reProxyAllocator.h"

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
}

/**
 * Default constructor initializes the world with the default settings
 */

reWorld::reWorld() : _broadPhase(nullptr), _allocator(nullptr), _integrator(nullptr) {
  SimpleAllocator* tmp = new SimpleAllocator();
  _allocator = new reProxyAllocator(tmp);
  _broadPhase = allocator().alloc_new<reBSPTree>(allocator());
  _integrator = allocator().alloc_new<re::Integrator>();
}

/**
 * Releases all resources used by the reWorld instance
 */

reWorld::~reWorld() {
  clear();
  
  allocator().alloc_delete(_broadPhase);
  allocator().alloc_delete(_integrator);
  
  if (_allocator != nullptr) {
    reBaseAllocator* al = ((reProxyAllocator*)_allocator)->allocator();
    delete _allocator;
    delete al;
  }
}

/**
 * Removes all entities in the reWorld
 */

void reWorld::clear() {
  _broadPhase->clear();
}

/**
 * Registers the entity to the world.
 * 
 * @param entity The entity to attach
 */

void reWorld::add(re::Entity& entity) {
  _broadPhase->add(entity);
}

/**
 * Removes the entity from the world. This does NOT deallocate the resources
 * associated with the entity.
 * 
 * @param entity The entity to remove
 */

void reWorld::remove(re::Entity& entity) {
  _broadPhase->remove(entity);
}

/**
 * Removes the entity from the world. This also deallocates the resources
 * associated with the entity, therefore, the reference will no longer be valid!
 * 
 * @param entity The entity to destroy
 */

void reWorld::destroy(re::Entity& entity) {
  remove(entity);
  allocator().alloc_delete(&entity.shape());
  allocator().alloc_delete(&entity);
}

/**
 * Advances the reWorld forward in time by the given time step
 * 
 * @param dt The time step to advance in user defined units
 */

void reWorld::advance(reFloat dt) {
  _broadPhase->advance(integrator(), dt);
}

/**
 * Returns a list of all re::Entity contained in the reWorld
 * 
 * @return A list of re::Entity
 */

const reLinkedList<re::Entity*>& reWorld::entities() const {
  return _broadPhase->entities();
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

re::Entity* reWorld::queryWithRay(const re::vec3& origin, const re::vec3& dir, re::vec3* intersect, re::vec3* normal) {
  re::RayQuery res = _broadPhase->queryWithRay(re::Ray(origin, dir));
  
  if (res.entity != nullptr) {
    if (intersect != nullptr) {
      *intersect = res.point;
    }
    if (normal != nullptr) {
      *normal = res.normal;
    }
    return res.entity;
  }
  
  return nullptr;
}

