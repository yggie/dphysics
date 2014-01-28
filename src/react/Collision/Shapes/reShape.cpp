#include "react/Collision/Shapes/reShape.h"

#include "react/math.h"
#include "react/Collision/reAABB.h"

using namespace re;

void reShape::updateAABB(const re::mat3& parentRotation) {
  _aabb.dimens().setZero();
  for (reUInt i = 0; i < numVerts(); i++) {
    const re::vec3 v = parentRotation * vert(i);
    
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
    result.normal = normalize(transform.multDir(result.normal));
    result.distSq = lengthSq(query.origin - result.intersect);
    return true;
  } else {
    return false;
  }
  
  return false;
}

bool reShape::intersectsHyperplane(const reTransform& transform, const reHyperplaneQuery& query) const {
  const reUInt N = numVerts();
  const reTransform inv = transform.inverse();
  const re::vec3 nPoint = inv.multPoint(query.point);
  const re::vec3 nDir = inv.multDir(query.dir);
  
  for (reUInt i = 0; i < N; i++) {
    if (dot(vert(i) - nPoint, nDir) + shell() > 0.0) {
      return true;
    }
  }
  
  return false;
}

