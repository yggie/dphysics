#ifndef RE_RIGIDBODY_H
#define RE_RIGIDBODY_H

#include "react/Solid.h"

namespace re {
  
  /**
   * A type of Solid with a non-deformable shell
   * 
   * @see Solid
   */
  
  class RigidBody : public Solid {
  public:
    RigidBody();
    virtual ~RigidBody();
    
    Ent::Type type() const;
    
    const vec vel() const override;
    const mat rotVel() const override;
    
    reFloat mass() const override;
    const mat inertia() const override;
    reFloat density() const override;
    
  protected:
    vec _vPos;
    mat _mRot;
    vec _vVel;
    mat _mRotVel;
    
    reFloat _sMass;
    mat _mInertia;
    reFloat _sDensity;
  };
  
  inline Ent::Type RigidBody::type() const {
    return RIGID;
  }
  
  inline const vec RigidBody::vel() const {
    return _vVel;
  }
  
  inline const mat RigidBody::rotVel() const {
    return _mRotVel;
  }
  
  inline reFloat RigidBody::mass() const {
    return _sMass;
  }
  
  inline const mat RigidBody::inertia() const {
    return _mInertia;
  }
  
  inline reFloat RigidBody::density() const {
    return _sDensity;
  }
}

#endif
