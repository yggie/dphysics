/**
 * @file
 * This file contains the definition of the reSphere class.
 */
#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/Collision/Shapes/reShape.h"

/**
 * @ingroup shapes
 * Represents a spherical reShape
 * 
 * @see reShape
 */

class reSphere : public reShape {
public:
  reSphere(reFloat radius);
  reSphere(const reSphere& sphere);
  ~reSphere();
  
  reFloat radius() const;
  Type type() const override;
  reUInt numVerts() const override;
  const re::vec3 vert(reUInt i) const override;
  reFloat volume() const override;
  const re::mat3 computeInertia() const override;
  
  void setRadius(reFloat radius);
  reSphere& withRadius(reFloat radius);
  
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;
};

/**
 * Returns the radius of the reSphere
 * 
 * @return The radius in user-defined units
 */

inline reFloat reSphere::radius() const {
  return _shell;
}

/**
 * Returns the identifier for the reShape type
 * 
 * @return Always returns reShape::SPHERE
 */

inline reShape::Type reSphere::type() const {
  return reShape::SPHERE;
}

inline reUInt reSphere::numVerts() const {
  return 1;
}

inline const re::vec3 reSphere::vert(reUInt) const {
  return re::vec3(0.0, 0.0, 0.0);
}

inline reFloat reSphere::volume() const {
  return RE_PI * 4.0 * radius()*radius()*radius() / 3.0;
}

inline const re::mat3 reSphere::computeInertia() const {
  return re::mat3(2.0 * radius() * radius() / 5.0);
}

/**
 * Set the radius of the reSphere
 * 
 * @param radius The new radius
 */

inline void reSphere::setRadius(reFloat radius) {
  _shell = radius;
}

/**
 * Set the radius of the reSphere, this method can be chained
 * 
 * @param radius The new radius
 * @return A reference to the reSphere
 */

inline reSphere& reSphere::withRadius(reFloat radius) {
  setRadius(radius);
  return *this;
}

#endif
