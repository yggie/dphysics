#ifndef RE_RIGIDBODY_H
#define RE_RIGIDBODY_H

#include "react/Solid.h"
#include "react/Shape.h"

namespace re {
  
  /**
   * @ingroup entities
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
    
    void setMass(reFloat mass) override;
    void setDensity(reFloat density) override;
    
  protected:
    /** The RigidBody's velocity */
    vec _vVel;
    /** The RigidBody's rotational velocity matrix */
    mat _mRotVel;
    
    /** The RigidBody's mass */
    reFloat _sMass;
    /** The RigidBody's inertia tensor */
    mat _mInertia;
    
    void updateInertia();
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
    return _sMass / _shape->volume();
  }
  
  inline void RigidBody::setMass(reFloat mass) {
    _sMass = mass;
    updateInertia();
  }
  
  inline void RigidBody::setDensity(reFloat density) {
    _sMass = density / _shape->volume();
    updateInertia();
  }
  
  inline void RigidBody::updateInertia() {
    _mInertia = _shape->computeInertia() * _sMass;
  }
}

#endif
