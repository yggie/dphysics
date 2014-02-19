#include "react/Utilities/Builder.h"

#include "react/reWorld.h"
#include "react/Memory/reAllocator.h"
#include "react/Collision/reBroadPhase.h"

#include "react/Entities/Rigid.h"
#include "react/Entities/Static.h"

#include "react/Collision/Shapes/shapes.h"

#include "react/Dynamics/reGravAction.h"

/**
 * Creates a new Rigid and properly initializes it into the reWorld
 * 
 * @return The attached Rigid entity
 */

re::Rigid& re::Builder::Rigid(const reShape& shape) {
  re::Rigid* body = _world.allocator().alloc_new<re::Rigid>(*copyOf(shape));
  _world.add(*body);
  return *body;
}

/**
 * Creates a new Rigid and properly initializes it into the reWorld
 * 
 * @return The attached Rigid entity
 */

re::Rigid& re::Builder::Rigid(const reShape& shape, const re::Transform& transform) {
  reShape* base = copyOf(shape);
  reShape* proxy = _world.allocator().alloc_new<re::ShapeProxy>(base, transform);
  re::Rigid* body = _world.allocator().alloc_new<re::Rigid>(*proxy);
  _world.add(*body);
  return *body;
}

re::Static& re::Builder::Static(const reShape& shape) {
  re::Static* body = _world.allocator().alloc_new<re::Static>(*copyOf(shape));
  _world.add(*body);
  return *body;
}

re::Static& re::Builder::Static(const reShape& shape, const re::Transform& transform) {
  reShape* base = copyOf(shape);
  reShape* proxy = _world.allocator().alloc_new<re::ShapeProxy>(base, transform);
  re::Static* body = _world.allocator().alloc_new<re::Static>(*proxy);
  _world.add(*body);
  return *body;
}

reGravAction& re::Builder::GravAction(Entity& A, Entity& B) {
  reGravAction* action = _world.allocator().alloc_new<reGravAction>();
  _world.broadPhase().addInteraction(*action, A, B);
  return *action;
}

reShape* re::Builder::copyOf(const reShape& shape) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return _world.allocator().alloc_new<re::Sphere>((const re::Sphere&)shape);

    case reShape::PLANE:
      return _world.allocator().alloc_new<re::Plane>((const re::Plane&)shape);
    
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

