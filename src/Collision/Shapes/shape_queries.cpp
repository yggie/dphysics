#include "react/Collision/Shapes/shapes.h"

#include "react/Collision/reSpatialQueries.h"

#include "react/debug.h"

bool sphereRayIntersect(const re::Sphere& sphere, const re::Ray& ray, re::Intersect& intersect) {
  const reFloat a = re::lengthSq(ray.dir());
  const reFloat b = 2 * re::dot(ray.origin(), ray.dir());
  const reFloat c = re::lengthSq(ray.origin()) - re::sq(sphere.radius());
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
  intersect.depth = re::length(intersect.point - ray.origin());
  
  return true;
}

/**
 * Computes the intersection data between a transformed shape and a
 * ray object
 *
 * @param shape The shape to test
 * @param transform The transform of the shape
 * @param ray The ray to test with
 * @param intersect A struct containing data on the intersection
 */

bool re::intersects(const reShape& shape, const re::Transform& transform, const re::Ray& ray, re::Intersect& intersect) {
  // TODO convert back to virtual methods, faster than switch
  switch (shape.type()) {
    case reShape::SPHERE:
      if (sphereRayIntersect((const re::Sphere&)shape, re::Ray(ray, re::inverse(transform)), intersect)) {
        intersect.point = transform.applyToPoint(intersect.point);
        intersect.normal = re::normalize(transform.applyToDir(intersect.normal));
        intersect.depth = re::length(ray.origin() - intersect.point);
        return true;
      }
      return false;

    case reShape::PROXY:
      {
        const reProxyShape& proxy = (const reProxyShape&)shape;
        return (re::intersects(*proxy.shape(), transform * proxy.transform(), ray, intersect)); 
      }

    default:
      RE_IMPOSSIBLE
      throw 0;
  }
}

/**
 * Returns an enum describing the relative location of the shape to the
 * given plane
 *
 * @param shape The shape object
 * @param plane The plane object
 */

re::Location re::relativeToPlane(const reShape& shape, const re::Plane& plane) {
  const reUInt N = shape.numVerts();
  reFloat maxV = RE_NEGATIVE_INFINITY;
  reFloat minV = RE_INFINITY;
  
  for (reUInt i = 0; i < N; i++) {
    const reFloat dat = re::dot(shape.vert(i), plane.normal()) - plane.offset();
    const reFloat mx = dat + shape.shell();
    const reFloat mn = dat - shape.shell();
    if (mx > maxV) {
      maxV = mx;
    }
    if (mn < minV) {
      minV = mn;
    }
    if ((maxV > RE_FP_TOLERANCE && minV < RE_FP_TOLERANCE) ||
        (maxV < RE_FP_TOLERANCE && minV > RE_FP_TOLERANCE)) {
      return re::INTERSECT;
    }
  }
  
  if (minV > RE_FP_TOLERANCE && maxV > RE_FP_TOLERANCE) {
    return re::FRONT;
  } else if (minV < RE_FP_TOLERANCE && maxV < RE_FP_TOLERANCE) {
    return re::BACK;
  }
  
  return re::INTERSECT;
}

/**
 * Returns an enum describing the relative location of the shape in the
 * given plane
 *
 * @param shape The shape object
 * @param transform The transform of the shape
 * @param plane The plane object
 */

re::Location re::relativeToPlane(const reShape& shape, const re::Transform& transform, const re::Plane& plane) {
  switch (shape.type()) {
    case reShape::SPHERE:
      return re::relativeToPlane(shape, re::Plane(plane, re::inverse(transform)));
      break;

    case reShape::PROXY:
      {
        const reProxyShape& proxy = (const reProxyShape&)shape;
        return re::relativeToPlane(*proxy.shape(), transform * proxy.transform(), plane);
      }
      break;

    default:
      RE_IMPOSSIBLE
      throw 0;
  }
}

bool intersects3(const re::Sphere& A, const re::Transform& tA, const re::Sphere& B, const re::Transform& tB, re::Intersect& intersect) {
  const reFloat minDist = A.radius() + B.radius();
  bool contact = (re::lengthSq(tA.v - tB.v) < re::sq(minDist));
  if (contact) {
    intersect.point = (tA.v + tB.v) / 2.0;
    intersect.normal = re::normalize(tA.v - tB.v);
    intersect.depth = re::length(tA.v - intersect.point);
  }
  
  return contact;
}

template <class S>
bool intersects2(const S& A, const re::Transform& tA, const reShape& B, const re::Transform& tB, re::Intersect& intersect) {
  switch (B.type()) {
    case reShape::SPHERE:
      return intersects3(A, tA, (const re::Sphere&)B, tB, intersect);
      break;

    default:
      RE_NOT_IMPLEMENTED
      throw 0;
  }
}

bool re::intersects(const reShape& A, const re::Transform& tA, const reShape& B, const re::Transform& tB, re::Intersect& intersect) {
  switch (A.type()) {
    case reShape::SPHERE:
      return intersects2((const re::Sphere&)A, tA, B, tB, intersect);
      break;

    default:
      RE_NOT_IMPLEMENTED
      throw 0;
  }
}

