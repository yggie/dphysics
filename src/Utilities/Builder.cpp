#include "react/Utilities/Builder.h"

#include "react/reWorld.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/reBroadPhase.h"

#include "react/Entities/reRigidBody.h"
#include "react/Entities/StaticBody.h"

#include "react/Collision/Shapes/shapes.h"

#include "react/Dynamics/reGravAction.h"

using namespace re;

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& Builder::RigidBody(const reShape& shape) {
  reRigidBody* body = _world.allocator().alloc_new<reRigidBody>(copyOf(shape));
  _world.add(*body);
  return *body;
}

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& Builder::RigidBody(const reShape& shape, const re::Transform& transform) {
  reShape* base = copyOf(shape);
  reShape* newShape = _world.allocator().alloc_new<reProxyShape>(base, transform);
  reRigidBody* body = _world.allocator().alloc_new<reRigidBody>(newShape);
  _world.add(*body);
  return *body;
}

re::StaticBody& Builder::StaticBody(const reShape& shape) {
  re::StaticBody* body = _world.allocator().alloc_new<re::StaticBody>(copyOf(shape));
  _world.add(*body);
  return *body;
}

re::StaticBody& Builder::StaticBody(const reShape& shape, const re::Transform& transform) {
  reShape* base = copyOf(shape);
  reShape* proxy = _world.allocator().alloc_new<reProxyShape>(base, transform);
  re::StaticBody* body = _world.allocator().alloc_new<re::StaticBody>(proxy);
  _world.add(*body);
  return *body;
}

reGravAction& Builder::GravAction(reEnt& A, reEnt& B) {
  reGravAction* action = _world.allocator().alloc_new<reGravAction>();
  _world.broadPhase().addInteraction(*action, A, B);
  return *action;
}

reShape* Builder::copyOf(const reShape& shape) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return _world.allocator().alloc_new<re::Sphere>((const re::Sphere&)shape);
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      break;
    
    case reShape::TRIANGLE:
      return _world.allocator().alloc_new<reTriangle>((const reTriangle&)shape);
    
    case reShape::PROXY:
      RE_NOT_IMPLEMENTED
      break;
  }
  
  RE_IMPOSSIBLE
  throw 0; // TODO proper exceptions
}

