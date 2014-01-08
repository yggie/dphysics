#include "react/common.h"

#include "react/Collision/Shapes/shapes.h"
#include "react/Collision/reSpatialQueries.h"

reAllocator* re::globalAllocator = nullptr;

reUInt re::globalQueryID = 0;

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
      {
        // TODO fix very bad bug with distorted shape unable to allocate shapes
        const reDistortedShape& orig = (const reDistortedShape&)shape;
        reDistortedShape* copy = re::alloc_new<reDistortedShape>();
        copy->setShape(orig.shape());
        copy->setDistortion(orig.distortion());
        return copy;
      }
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
