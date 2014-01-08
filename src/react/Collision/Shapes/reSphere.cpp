#include "react/Collision/Shapes/reSphere.h"

#include "react/Entities/reEnt.h"

reSphere::reSphere(reFloat radius) : reShape() {
  _shell = radius;
}

reSphere::reSphere(const reSphere& sphere) : reShape() {
  _shell = sphere.radius();
}

reSphere::~reSphere() {
  // do nothing
}

bool reSphere::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  // solve for the points of intersection
  const reFloat a = query.dir.lengthSq();
  const reFloat b = 2 * query.origin.dot(query.dir);
  const reFloat c = query.origin.lengthSq() - radius() * radius();
  const reFloat discriminant = b*b - 4*a*c;
  
  if (discriminant < RE_FP_TOLERANCE) {
    return false;
  }
  
  // consider only the smaller of the two solutions
  const reFloat solA = (-b - reSqrt(discriminant)) / (2.0 * a);
  
  // invalid solutions
  if (solA < RE_FP_TOLERANCE) {
    return false;
  }
  
  result.intersect = query.origin + query.dir * solA;
  result.normal = (query.origin + query.dir * solA).normalized();
  result.distSq = (result.intersect - query.origin).lengthSq();
  
  return true;
}

