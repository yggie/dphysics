/**
 * @file
 * Contains definitions for query functions between shapes
 */
#ifndef RE_SHAPE_QUERIES_H
#define RE_SHAPE_QUERIES_H

#include "react/math.h"

namespace re {

  class Ray;
  class Plane;
  class Segment;

  struct Intersect {
    Intersect() : point(), normal() { }
    reFloat depth;
    re::vec3 point;
    re::vec3 normal;
  };
  
  bool contains(const reShape& shape, const reTransform& transform, const re::vec3& point);

  bool intersects(const reShape& shape, const reTransform& transform, const re::Ray& ray, Intersect& intersect);

  bool intersects(const reShape& A, const reTransform& tA, const reShape& B, const reTransform& tB, Intersect& intersect);
}

#endif
