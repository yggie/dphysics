#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/Shape.h"

namespace re {
  
  /**
   * Represents a spherical Shape
   * 
   * @see Shape
   */
  
  class Sphere : public Shape {
  public:
    Sphere(reFloat radius);
    Sphere(const Sphere& sphere);
    ~Sphere();
    
    Type type() const override;
    reFloat volume() const override;
    
  protected:
    reFloat _sRadius;
  };
  
  inline Shape::Type Sphere::type() const {
    return Shape::SPHERE;
  }
  
  inline reFloat Sphere::volume() const {
    return PI * 4.0 * _sRadius*_sRadius*_sRadius / 3.0;
  }
}

#endif
