#ifndef RE_RIGIDBODY_H
#define RE_RIGIDBODY_H

#include "react/absbody.h"

namespace re {
  
  /**
   * A subset of AbsBody, representing bodies with non-deformable shells
   * 
   * @see AbsBody
   */
  
  class RigidBody : public AbsBody {
  public:
    RigidBody();
    virtual ~RigidBody();
    
    Entity::Type type() const;
    
    const vec pos() const override;
    const mat rot() const override;
    const vec vel() const override;
    const mat rotVel() const override;
    
    reFloat mass() const override;
    const mat inertia() const override;
    
  protected:
    vec _vPos;
    mat _mRot;
    vec _vVel;
    mat _mRotVel;
    
    reFloat _sMass;
    mat _mInertia;
  };
  
  inline Entity::Type RigidBody::type() const {
    return RIGID;
  }
  
  inline const vec RigidBody::pos() const {
    return _vPos;
  }
  
  inline const mat RigidBody::rot() const {
    return _mRot;
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
}

#endif
