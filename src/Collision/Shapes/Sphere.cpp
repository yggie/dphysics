#include "react/Collision/Shapes/Sphere.h"

#include "react/Entities/reEnt.h"

using namespace re;

Sphere::Sphere(reFloat radius) : reShape() {
  _shell = radius;
}

Sphere::Sphere(const Sphere& sphere) : reShape() {
  _shell = sphere.radius();
}

Sphere::~Sphere() {
  // do nothing
}

const re::vec3 Sphere::randomPoint() const {
  return re::normalize(re::vec3::rand()) * re::randf(0.0, 0.99) * radius();
}

bool Sphere::containsPoint(const re::vec3& point) const {
  return lengthSq(point) < radius()*radius();
}

/**
bool Sphere::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  // solve for the points of intersection
  const reFloat a = re::lengthSq(query.dir);
  const reFloat b = 2 * re::dot(query.origin, query.dir);
  const reFloat c = re::lengthSq(query.origin) - radius() * radius();
  const reFloat discriminant = b*b - 4*a*c;
  
  if (discriminant < RE_FP_TOLERANCE) {
    return false;
  }
  
  // consider only the smaller of the two solutions
  const reFloat solA = (-b - re::sqrt(discriminant)) / (2.0 * a);
  
  // invalid solutions
  if (solA < RE_FP_TOLERANCE) {
    return false;
  }
  
  result.intersect = query.origin + query.dir * solA;
  result.normal = re::normalize(query.origin + query.dir * solA);
  result.distSq = re::lengthSq(result.intersect - query.origin);
  
  return true;
}
*/
