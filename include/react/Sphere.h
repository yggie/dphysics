#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/matrix.h"
#include "react/Shape.h"

namespace re {
  
  /**
   * @ingroup shapes
   * Represents a spherical Shape
   * 
   * @see Shape
   */
  
  class Sphere : public Shape {
  public:
    Sphere(reFloat radius);
    Sphere(const Sphere& sphere);
    ~Sphere();
    
    reFloat radius() const;
    Type type() const override;
    reFloat volume() const override;
    const mat computeInertia() const override;
    
    Sphere& withRadius(reFloat radius);
    
  protected:
    reFloat _sRadius;
  };
  
  inline reFloat Sphere::radius() const {
    return _sRadius;
  }
  
  inline Shape::Type Sphere::type() const {
    return Shape::SPHERE;
  }
  
  inline reFloat Sphere::volume() const {
    return PI * 4.0 * _sRadius*_sRadius*_sRadius / 3.0;
  }
  
  inline const mat Sphere::computeInertia() const {
    return mat(2.0 * _sRadius * _sRadius / 5.0);
  }
  
  inline Sphere& Sphere::withRadius(reFloat radius) {
    _sRadius = radius;
    return *this;
  }
}

#endif
