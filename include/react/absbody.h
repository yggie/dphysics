#ifndef RE_ABSBODY_H
#define RE_ABSBODY_H

#include "react/entity.h"

namespace re {
  
  /**
   * A subset of Entity which contains mass
   * 
   * @see Entity
   */
  
  class AbsBody : public Entity {
  public:
    virtual ~AbsBody();
    
    virtual float mass() const = 0;
    virtual const mat inertia() const = 0;
  };
  
  inline AbsBody::~AbsBody() { }
}

#endif
