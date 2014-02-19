#include "react/Entities/Entity.h"

#include "react/Collision/Shapes/shapes.h"

using namespace re;

re::ID Entity::globalEntID = 1;

/**
 * Returns true if the Entity intersects the ray specified
 * 
 * @param ray The ray object
 * @param intersect The intersect result struct
 */

bool Entity::intersects(const re::Ray& ray, re::Intersect& intersect) const {
  return re::intersects(_shape, transform(), ray, intersect);
}

re::Location Entity::relativeToPlane(const re::Plane& plane) {
  return re::relativeToPlane(_shape, transform(), plane);
}
