#include "react/Collision/Shapes/reShape.h"

#include "react/math.h"
#include "react/Collision/reAABB.h"

void reShape::updateAABB(const re::mat3& parentRotation) {
  _aabb.dimens().setZero();
  for (reUInt i = 0; i < numVerts(); i++) {
    const re::vec3 v = parentRotation * vert(i);
    
    for (reUInt j = 0; j < 3; j++) {
      const reFloat d = re::abs(v[j]) + shell();
      if (d > _aabb.dimens()[j]) {
        _aabb.dimens()[j] = d;
      }
    }
  }
}

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

re::PlaneQuery::FastResult reShape::fastPlaneIntersect(const re::vec3& normal, const re::vec3& center) const {
  const reUInt N = numVerts();
  reFloat maxV = 0.0;
  reFloat minV = 0.0;
  
  for (reUInt i = 0; i < N; i++) {
    const reFloat dat = re::dot(vert(i) - center, normal);
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
      return re::PlaneQuery::INTERSECTS;
    }
  }
  
  if (minV > RE_FP_TOLERANCE && maxV > RE_FP_TOLERANCE) {
    return re::PlaneQuery::FRONT;
  } else if (minV < RE_FP_TOLERANCE && maxV < RE_FP_TOLERANCE) {
    return re::PlaneQuery::BEHIND;
  }
  
  return re::PlaneQuery::INTERSECTS;
}

