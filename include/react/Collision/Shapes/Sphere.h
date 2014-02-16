/**
 * @file
 * This file contains the definition of the Sphere class.
 */
#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/Collision/Shapes/reShape.h"

namespace re {
  /**
   * @ingroup shapes
   * Represents a spherical reShape
   * 
   * @see reShape
   */

  class Sphere : public reShape {
  public:
    Sphere(reFloat radius);
    Sphere(const Sphere& sphere);
    ~Sphere();
    
    reFloat radius() const;
    Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;
    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    
    void setRadius(reFloat radius);
    Sphere& withRadius(reFloat radius);
    
    // utility methods
    const re::vec3 randomPoint() const override;
    
    // collision queries
    bool containsPoint(const re::vec3& point) const override;
  };

  /**
   * Returns the radius of the Sphere
   * 
   * @return The radius in user-defined units
   */

  inline reFloat Sphere::radius() const {
    return _shell;
  }

  /**
   * Returns the identifier for the reShape type
   * 
   * @return Always returns reShape::SPHERE
   */

  inline reShape::Type Sphere::type() const {
    return reShape::SPHERE;
  }

  inline reUInt Sphere::numVerts() const {
    return 1;
  }

  inline const re::vec3 Sphere::vert(reUInt) const {
    return re::vec3(0.0, 0.0, 0.0);
  }

  inline reFloat Sphere::volume() const {
    return RE_PI * 4.0 * radius()*radius()*radius() / 3.0;
  }

  inline const re::mat3 Sphere::computeInertia() const {
    return re::mat3(2.0 * radius() * radius() / 5.0);
  }

  /**
   * Set the radius of the Sphere
   * 
   * @param radius The new radius
   */

  inline void Sphere::setRadius(reFloat radius) {
    _shell = radius;
  }

  /**
   * Set the radius of the Sphere, this method can be chained
   * 
   * @param radius The new radius
   * @return A reference to the Sphere
   */

  inline Sphere& Sphere::withRadius(reFloat radius) {
    setRadius(radius);
    return *this;
  }
}

#endif
