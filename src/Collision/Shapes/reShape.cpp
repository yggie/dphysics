#include "react/Collision/Shapes/reShape.h"

#include "react/math.h"
#include "react/Collision/reAABB.h"

bool reShape::containsPoint(const re::Transform& transform, const re::vec3& point) const {
  return containsPoint(transform.applyToPoint(point));
}

/**
bool reShape::intersectsRay(const re::Transform& transform, const reRayQuery& query, reRayQueryResult& result) const {
  const re::Transform inv = transform.inverse();
  reRayQuery newQuery;
  newQuery.origin = inv.applyToPoint(query.origin);
  newQuery.dir = inv.applyToDir(query.dir);

  if (intersectsRay(newQuery, result)) {
    result.intersect = transform.applyToPoint(result.intersect);
    result.normal = normalize(transform.applyToDir(result.normal));
    result.distSq = lengthSq(query.origin - result.intersect);
    return true;
  } else {
    return false;
  }
  
  return false;
}


*/
