#include "react/Utilities/reBuilder.h"

#include "react/reWorld.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/reBroadPhase.h"

#include "react/Entities/reRigidBody.h"

#include "react/Collision/Shapes/shapes.h"

#include "react/Dynamics/reGravAction.h"

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& reBuilder::RigidBody(const reShape& shape) {
  reRigidBody* body = _world.allocator().alloc_new<reRigidBody>(copyOf(shape));
  _world.add(*body);
  return *body;
}

/**
 * Creates a new reRigidBody and properly initializes it into the reWorld
 * 
 * @return The attached reRigidBody entity
 */

reRigidBody& reBuilder::RigidBody(const reShape& shape, const reTransform& transform) {
  reShape* base = copyOf(shape);
  reShape* newShape = _world.allocator().alloc_new<reProxyShape>(base, transform);
  reRigidBody* body = _world.allocator().alloc_new<reRigidBody>(newShape);
  _world.add(*body);
  return *body;
}

reGravAction& reBuilder::GravAction(reEnt& A, reEnt& B) {
  reGravAction* action = _world.allocator().alloc_new<reGravAction>();
  _world.broadPhase().addInteraction(*action, A, B);
  return *action;
}

reShape* reBuilder::copyOf(const reShape& shape) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return _world.allocator().alloc_new<reSphere>((const reSphere&)shape);
    
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

