/**
 * @file
 * Contains definitions for query functions between shapes
 */
#ifndef RE_SHAPE_QUERIES_H
#define RE_SHAPE_QUERIES_H

#include "react/math.h"

class reShape;

namespace re {

  class Ray;
  class Plane;
  class Segment;

  struct Intersect {
    Intersect() : depth(RE_INFINITY), point(), normal() { }
    reFloat depth;
    re::vec3 point;
    re::vec3 normal;
  };

  enum Location {
    FRONT,
    INTERSECT,
    BACK
  };
  
  bool intersects(const reShape& shape, const re::Transform& transform, const re::Ray& ray, Intersect& intersect);

  Location relativeToPlane(const reShape& shape, const re::Transform& transform, const re::Plane& plane);

  bool intersects(const reShape& A, const re::Transform& tA, const reShape& B, const re::Transform& tB, Intersect& intersect);
}

#endif
