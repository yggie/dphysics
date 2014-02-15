#include "react/Collision/Shapes/shapes.h"

bool sphereIntersect(const re::Sphere& sphere, re::Ray& ray, re::Intersect& intersect) {
  const reFloat a = re::lengthSq(ray.dir());
  const reFloat b = 2 * re::dot(ray.origin(), ray.dir());
  const reFloat c = re::lengthSq(ray.origin()) - radius() * radius();
  const reFloat discriminant = b*b - 4*a*c;
  
  if (discriminant < RE_FP_TOLERANCE) {
    return false;
  }
  
  // consider only the smaller of the two solutions
  const reFloat solA = (-b - re::sqrt(discriminant)) / (2.0 * a);
  
  // invalid solutions
  if (solA < RE_FP_TOLERANCE) {
    return false;
  }
  
  intersect.point = ray.origin() + ray.dir() * solA;
  intersect.normal = re::normalize(ray.origin() + ray.dir() * solA);
  intersect.dist = re::length(result.intersect - ray.origin());
  
  return true;
}

bool re::intersects(const reShape& shape, const reTransform& transform, const re::Ray& ray, re::Intersect& intersect) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return sphereIntersect((const re::Sphere&)shape, re::Ray(ray, transform), intersect);
      break;

    default:
      RE_IMPOSSIBLE
      throw 0;
  }
}

re::Location re::relativeToPlane(const reShape& shape, const reTransform& transform, const re::Plane& plane) {
  throw 0;
}

bool re::intersects(const reShape& A, const reTransform& tA, const reShape& B, const reTransform& tB, re::Intersect& intersect) {
}
