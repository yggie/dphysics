#include "react/Collision/Shapes/PlaneShape.h"

using namespace re;

const re::vec3 PlaneShape::randomPoint() const {
  return center() + re::randf(1000.0) * re::normalize(re::cross(normal(), re::vec3::rand()));
}

/// NOT TESTED
bool PlaneShape::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  if (re::abs(re::dot(normal(), query.dir) - 1.0) < RE_FP_TOLERANCE) {
    result.normal = normal();
    return true;
  }
  return false;
}

/// NOT TESTED
re::Plane::Location PlaneShape::locationInPlane(const re::Plane& plane) const {
  if (re::abs(re::dot(normal(), plane.normal()) - 1.0) < RE_FP_TOLERANCE) {
    if (offset() > plane.offset()) {
      return re::Plane::FRONT_OF_PLANE;
    } else {
      return re::Plane::BACK_OF_PLANE;
    }
  } else {
    return re::Plane::ON_PLANE;
  }
}
