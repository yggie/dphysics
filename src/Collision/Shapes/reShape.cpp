#include "react/Collision/Shapes/reShape.h"

#include "react/math.h"
#include "react/Collision/reAABB.h"

bool reShape::containsPoint(const reTransform& transform, const re::vec3& point) const {
  return containsPoint(transform.multPoint(point));
}

bool reShape::intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const {
  const reTransform inv = transform.inverse();
  reRayQuery newQuery;
  newQuery.origin = inv.multPoint(query.origin);
  newQuery.dir = inv.multDir(query.dir);

  if (intersectsRay(newQuery, result)) {
    result.intersect = transform.multPoint(result.intersect);
    result.normal = normalize(transform.multDir(result.normal));
    result.distSq = lengthSq(query.origin - result.intersect);
    return true;
  } else {
    return false;
  }
  
  return false;
}

re::Plane::Location reShape::locationInPlane(const re::Plane& plane) const {
  const reUInt N = numVerts();
  reFloat maxV = RE_NEGATIVE_INFINITY;
  reFloat minV = RE_INFINITY;
  
  for (reUInt i = 0; i < N; i++) {
    const reFloat dat = re::dot(vert(i), plane.normal()) - plane.offset();
    const reFloat mx = dat + shell();
    const reFloat mn = dat - shell();
    if (mx > maxV) {
      maxV = mx;
    }
    if (mn < minV) {
      minV = mn;
    }
    if ((maxV > RE_FP_TOLERANCE && minV < RE_FP_TOLERANCE) ||
        (maxV < RE_FP_TOLERANCE && minV > RE_FP_TOLERANCE)) {
      return re::Plane::ON_PLANE;
    }
  }
  
  if (minV > RE_FP_TOLERANCE && maxV > RE_FP_TOLERANCE) {
    return re::Plane::FRONT_OF_PLANE;
  } else if (minV < RE_FP_TOLERANCE && maxV < RE_FP_TOLERANCE) {
    return re::Plane::BACK_OF_PLANE;
  }
  
  return re::Plane::ON_PLANE;
}

