#ifndef RE_SPHERE_H
#define RE_SPHERE_H

#include "react/absshape.h"

namespace re {
  class Sphere : public AbsShape {
  public:
    Sphere();
    virtual ~Sphere();
    
    reFloat volume() const override;
    
  protected:
    reFloat _sRadius;
  };
  
  inline reFloat Sphere::volume() const {
    return PI * 4.0 * _sRadius*_sRadius*_sRadius / 3.0;
  }
}

#endif
