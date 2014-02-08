#include "react/Collision/Shapes/Plane.h"

using namespace re;

bool Plane::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  const reFloat dotProduct = re::dot(query.dir, _normal);
  if (re::abs(dotProduct - 1.0) < RE_FP_TOLERANCE) {
    return false;
  }

  const reFloat originOffset = re::dot(_normal, query.origin);
  result.intersect = query.origin - originOffset / dotProduct;
  result.distSq = re::dot(query.dir, result.intersect - query.origin);
  result.normal = - re::sign(dotProduct) * _normal;
  return true;
}

re::PlaneQuery::FastResult Plane::fastPlaneIntersect(const re::vec3& normal, const re::vec3& center) const {
  if (re::abs(re::dot(normal, _normal) - 1.0) < RE_FP_TOLERANCE) {
    if (re::dot(normal, center) > _offset) {
      return re::PlaneQuery::FRONT;
    } else {
      return re::PlaneQuery::BEHIND;
    }
  }
  return re::PlaneQuery::INTERSECTS;
}
