#include "react/Collision/Shapes/Segment.h"

using namespace re;

bool Segment::containsPoint(const re::vec3& point) const {
  return re::abs(re::dot(point - _origin, _dir)) < RE_FP_TOLERANCE;
}

bool Segment::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  const re::vec3 d = re::normalize(re::cross(query.dir, _dir));
  const reFloat dist = re::dot(d, _origin - query.origin);
  if (re::abs(dist) > RE_FP_TOLERANCE) {
    return false;
  }

  RE_NOT_IMPLEMENTED
  result.intersect = _origin + _dir * dist / re::dot(d, _dir);
  result.normal = d;
  result.distSq = re::sq(dist);
  return true;
}

re::PlaneQuery::FastResult Segment::fastPlaneIntersect(const re::vec3& normal, const re::vec3& center) const {
  if (re::abs(re::dot(normal, _dir)) < RE_FP_TOLERANCE) {
    return (re::dot(_origin - center, normal) > 0.0) ? re::PlaneQuery::FRONT : re::PlaneQuery::BEHIND;
  }

 return re::PlaneQuery::INTERSECTS; 
}

