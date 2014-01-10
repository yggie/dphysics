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

bool reShape::intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const {
  const reTransform inv = transform.inverse();
  reRayQuery newQuery;
  newQuery.origin = inv.multPoint(query.origin);
  newQuery.dir = inv.multDir(query.dir);

  if (intersectsRay(newQuery, result)) {
    result.intersect = transform.multPoint(result.intersect);
    result.normal = transform.multDir(result.normal).normalized();
    result.distSq = (query.origin - result.intersect).lengthSq();
    return true;
  } else {
    return false;
  }
  
  return false;
}

bool reShape::intersectsHyperplane(const reTransform& transform, const reHyperplaneQuery& query) const {
  const reUInt N = numVerts();
  const reTransform inv = transform.inverse();
  const reVector nPoint = inv.multPoint(query.point);
  const reVector nDir = inv.multDir(query.dir);
  
  for (reUInt i = 0; i < N; i++) {
    if ((vert(i) - nPoint).dot(nDir) + shell() > 0.0) {
      return true;
    }
  }
  
  return false;
}

