#include "react/Collision/Shapes/reTriangle.h"

bool reTriangle::intersectsRay(const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) const {
  const reVector u = (_verts[0] - _verts[1]);
  const reVector v = (_verts[2] - _verts[1]);
  
  const reVector faceNormal = v.cross(u).normalized();
  
  // parallel to plane
  if (reIsGreaterThanOrEqualZero(faceNormal.dot(dir))) {
    return false;
  }
  
  const reFloat lambda = (faceNormal.dot(_verts[0]) - faceNormal.dot(origin)) / faceNormal.dot(dir);
  
  // The solution is not in the direction of the ray
  if (reIsLessThanOrEqualZero(lambda)) {
    return false;
  }
  
  // compute ray-plane intersection
  const reVector intersection = origin + dir*lambda;
  
  // compute parametric coordinates
  reVector w = intersection - _verts[1];
  if (reIsAlmostZero(w.lengthSq())) {
    // TODO handle the degenerate case properly
    w = intersection - _verts[0];
    if (reIsAlmostZero(w.lengthSq())) {
      w = intersection - _verts[2];
    }
  }
  
  const reFloat uw = u.dot(w);
  const reFloat uv = u.dot(v);
  const reFloat vw = v.dot(w);
  const reFloat uu = u.lengthSq();
  const reFloat vv = v.lengthSq();
  
  const reFloat denom = uv*uv - uu*vv;
  const reFloat s = (uv*vw - vv*uw)/denom;
  const reFloat t = (uv*uw - uu*vw)/denom;
  
  if (reIsGreaterThanOrEqualZero(s) &&
      reIsGreaterThanOrEqualZero(t) &&
      reIsLessThanOrEqual((s + t), 1.0)) {
    if (intersect != nullptr) {
      *intersect = intersection;
    }
    if (normal != nullptr) {
      if (faceNormal.dot(dir) > 0.0) {
        *normal = -faceNormal;
      } else {
        *normal = faceNormal;
      }
    }
    return true;
  }
  
//    printf("COMPUTED (s, t, dn) = (%.5f, %.5f, %.5f)\n", s, t, denom);
//    printf("COMPUTED uw = %.5f, vw = %.5f)\n", uw, vw);
//    printf("COMPUTED uu%.5f, uv%.5f, uw%.5f, vw%.5f, vv%.5f)\n", uu, uv, uw, vw, vv);
//    printf("COMPUTED dir = (%.5f, %.5f, %.5f)\n", dir[0], dir[1], dir[2]);
  
//  RE_ASSERT(!reIsNan(s) || !reIsNan(t), "Cannot be NaN");
  
  return false;
}
