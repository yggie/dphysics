#include "react/common.h"

#include "react/Collision/Shapes/shapes.h"

reAllocator* re::globalAllocator = nullptr;

/**
 * A convenient method to create copies of shapes
 */

reShape* re::copyOf(const reShape& shape) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return re::alloc_new<reSphere>((const reSphere&)shape);
    
    case reShape::RECTANGLE:
      RE_NOT_IMPLEMENTED
      return nullptr;
    
    case reShape::COMPOUND:
      RE_NOT_IMPLEMENTED
      return nullptr;
    
    case reShape::TRIANGLE:
      return re::alloc_new<reTriangle>((const reTriangle&)shape);
    
    case reShape::DISTORTED:
      return re::alloc_new<reDistortedShape>((const reDistortedShape&)shape);
  }
  
  RE_IMPOSSIBLE
  return nullptr;
}

template <> void re::alloc_delete<reShape>(reShape* shape) {
  switch (shape->type()) {
    case reShape::SPHERE:
      re::alloc_delete<reSphere>((reSphere*)shape);
      return;
    
    case reShape::RECTANGLE:
      break;
    
    case reShape::COMPOUND:
      break;
    
    case reShape::TRIANGLE:
      re::alloc_delete<reTriangle>((reTriangle*)shape);
      return;
    
    case reShape::DISTORTED:
      re::alloc_delete<reDistortedShape>((reDistortedShape*)shape);
      return;
  }
  
  RE_IMPOSSIBLE
}
