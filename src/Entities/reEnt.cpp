#include "react/Entities/reEnt.h"

#include "react/Collision/Shapes/shapes.h"

reUInt reEnt::globalEntID = 1;

/**
 * Returns true if the reEnt intersects the ray specified
 * 
 * @param ray The ray object
 * @param intersect The intersect result struct
 */

bool reEnt::intersects(const re::Ray& ray, re::Intersect& intersect) const {
  return re::intersects(*_shape, transform(), ray, intersect);
}

re::Location reEnt::relativeToPlane(const re::Plane& plane) {
  return re::relativeToPlane(*_shape, transform(), plane);
}
