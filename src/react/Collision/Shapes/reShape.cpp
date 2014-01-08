#include "react/Collision/Shapes/reShape.h"

#include "react/math.h"
#include "react/Collision/reAABB.h"

void reShape::updateAABB(const reMatrix& parentRotation) {
  _aabb.dimens().setZero();
  for (reUInt i = 0; i < numVerts(); i++) {
    const reVector v = parentRotation * vert(i);
    
    for (reUInt j = 0; j < 3; j++) {
      const reFloat d = reAbs(v[j]) + shell();
      if (d > _aabb.dimens()[j]) {
        _aabb.dimens()[j] = d;
      }
    }
  }
}

bool reShape::intersectsRay(const reTMatrix& transform, const reRayQuery& query, reRayQueryResult& result) const {
  const reTMatrix inverse = transform.inverse();
  reRayQuery newQuery = query;
  newQuery.origin = inverse.mult(newQuery.origin, 1.0);
  newQuery.dir = inverse.mult(newQuery.dir, 0.0);
  
  if (intersectsRay(newQuery, result)) {
    result.intersect = transform.mult(result.intersect, 1.0);
    result.normal = transform.mult(result.normal, 0.0).normalized();
    result.distSq = (query.origin - result.intersect).lengthSq();
    return true;
  } else {
    return false;
  }
  
  return false;
}

bool reShape::intersectsHyperplane(const reTMatrix& transform, const reHyperplaneQuery& query) const {
  const reUInt N = numVerts();
  const reTMatrix inv = transform.inverse();
  const reVector nPoint = inv.mult(query.point, 1.0);
  const reVector nDir = inv.mult(query.dir, 0.0);
  
  for (reUInt i = 0; i < N; i++) {
    if ((vert(i) - nPoint).dot(nDir) + shell() > 0.0) {
      return true;
    }
  }
  
  return false;
}

