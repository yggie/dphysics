#include "react/Collision/Shapes/reTriangle.h"

using namespace re;

bool reTriangle::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  const re::vec3 u = (_verts[0] - _verts[1]);
  const re::vec3 v = (_verts[2] - _verts[1]);
  
  const re::vec3 faceNormal = normalize(cross(v, u));
  
  // parallel to plane
  if (dot(faceNormal, query.dir) > RE_FP_TOLERANCE) {
    return false;
  }
  
  const reFloat lambda = (dot(faceNormal, _verts[0]) - dot(faceNormal, query.origin)) / dot(faceNormal, query.dir);
  
  // The solution is not in the direction of the ray
  if (lambda < RE_FP_TOLERANCE) {
    return false;
  }
  
  // compute ray-plane intersection
  const re::vec3 intersection = query.origin + query.dir*lambda;
  
  // compute parametric coordinates
  re::vec3 w = intersection - _verts[1];
  if (lengthSq(w) < RE_FP_TOLERANCE) {
    // TODO handle the degenerate case properly
    w = intersection - _verts[0];
    if (lengthSq(w) < RE_FP_TOLERANCE) {
      w = intersection - _verts[2];
    }
  }
  
  const reFloat uw = dot(u, w);
  const reFloat uv = dot(u, v);
  const reFloat vw = dot(v, w);
  const reFloat uu = dot(u, u);
  const reFloat vv = dot(v, v);
  
  const reFloat denom = uv*uv - uu*vv;
  const reFloat s = (uv*vw - vv*uw)/denom;
  const reFloat t = (uv*uw - uu*vw)/denom;
  
  if ( (s > RE_FP_TOLERANCE) && (t > RE_FP_TOLERANCE) &&
      (s + t < 1.0 - RE_FP_TOLERANCE) ) {
    result.intersect = intersection;
    if (dot(faceNormal, query.dir) > 0.0) {
      result.normal = -faceNormal;
    } else {
      result.normal = faceNormal;
    }
    result.distSq = lengthSq(query.origin - result.intersect);
    return true;
  }
  
//    printf("COMPUTED (s, t, dn) = (%.5f, %.5f, %.5f)\n", s, t, denom);
//    printf("COMPUTED uw = %.5f, vw = %.5f)\n", uw, vw);
//    printf("COMPUTED uu%.5f, uv%.5f, uw%.5f, vw%.5f, vv%.5f)\n", uu, uv, uw, vw, vv);
//    printf("COMPUTED dir = (%.5f, %.5f, %.5f)\n", dir[0], dir[1], dir[2]);
  
//  RE_ASSERT(!reIsNan(s) || !reIsNan(t), "Cannot be NaN");
  
  return false;
}
