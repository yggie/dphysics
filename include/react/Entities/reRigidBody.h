/**
 * @file
 * Contains the definition for the reRigidBody class
 */
#ifndef RE_RIGIDBODY_H
#define RE_RIGIDBODY_H

#include "react/Entities/reSolid.h"

/**
 * @ingroup entities
 * A type of reSolid with a non-deformable shell
 * 
 * @see reSolid
 */

class reRigidBody : public reSolid {
public:
  reRigidBody();
  reRigidBody(const reRigidBody&) = delete;
  virtual ~reRigidBody();
  
  reRigidBody& operator=(const reRigidBody&) = delete;
  
  reEnt::Type type() const;
  
  // getters for state properties
  const reVector vel() const override;
  const reMatrix rotVel() const override;
  
  // getters for material properties
  reFloat mass() const override;
  const reMatrix inertia() const override;
  reFloat density() const override;
  
  // setters for material properties
  void setMass(reFloat mass) override;
  void setDensity(reFloat density) override;
  reRigidBody& at(const reVector& position) override;
  reRigidBody& at(reFloat x, reFloat y, reFloat z) override;
  reRigidBody& withShape(const reShape& shape) override;
  reRigidBody& withMass(reFloat mass) override;
  reRigidBody& withDensity(reFloat mass) override;
  
protected:
  /** The reRigidBody's velocity */
  reVector _vVel;
  /** The reRigidBody's rotational velocity matrix */
  reMatrix _mRotVel;
  
  /** The reRigidBody's mass */
  reFloat _sMass;
  /** The reRigidBody's inertia tensor */
  reMatrix _mInertia;
  
  void updateInertia();
};

inline reEnt::Type reRigidBody::type() const {
  return RIGID;
}

inline const reVector reRigidBody::vel() const {
  return _vVel;
}

inline const reMatrix reRigidBody::rotVel() const {
  return _mRotVel;
}

inline reFloat reRigidBody::mass() const {
  return _sMass;
}

inline const reMatrix reRigidBody::inertia() const {
  return _mInertia;
}

inline reFloat reRigidBody::density() const {
  return _sMass / _shape->volume();
}

inline void reRigidBody::setMass(reFloat mass) {
  _sMass = mass;
  updateInertia();
}

inline void reRigidBody::setDensity(reFloat density) {
  _sMass = density / _shape->volume();
  updateInertia();
}

inline void reRigidBody::updateInertia() {
  _mInertia = _shape->computeInertia() * _sMass;
}

inline reRigidBody& reRigidBody::at(const reVector& position) {
  setPos(position);
  return *this;
}

inline reRigidBody& reRigidBody::at(reFloat x, reFloat y, reFloat z) {
  setPos(x, y, z);
  return *this;
}

inline reRigidBody& reRigidBody::withShape(const reShape& shape) {
  setShape(shape);
  return *this;
}

inline reRigidBody& reRigidBody::withMass(reFloat mass) {
  setMass(mass);
  return *this;
}

inline reRigidBody& reRigidBody::withDensity(reFloat density) {
  setDensity(density);
  return *this;
}

#endif
