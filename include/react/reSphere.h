/**
 * @file
 * This file contains the definition of the reSphere class.
 */
#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/reMatrix.h"
#include "react/reShape.h"

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
  reFloat volume() const override;
  const reMatrix computeInertia() const override;
  
  void setRadius(reFloat radius);
  reSphere& withRadius(reFloat radius);
  
protected:
  reFloat _sRadius;
};

/**
 * Returns the radius of the reSphere
 * 
 * @return The radius in user-defined units
 */

inline reFloat reSphere::radius() const {
  return _sRadius;
}

/**
 * Returns the identifier for the reShape type
 * 
 * @return Always returns reShape::SPHERE
 */

inline reShape::Type reSphere::type() const {
  return reShape::SPHERE;
}

inline reFloat reSphere::volume() const {
  return RE_PI * 4.0 * _sRadius*_sRadius*_sRadius / 3.0;
}

inline const reMatrix reSphere::computeInertia() const {
  return reMatrix(2.0 * _sRadius * _sRadius / 5.0);
}

/**
 * Set the radius of the reSphere
 * 
 * @param radius The new radius
 */

inline void reSphere::setRadius(reFloat radius) {
  _sRadius = radius;
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
