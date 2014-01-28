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
  reRigidBody(reShape* shape);
  reRigidBody(const reRigidBody&) = delete;
  virtual ~reRigidBody();
  
  reRigidBody& operator=(const reRigidBody&) = delete;
  
  reEnt::Type type() const;
  
  void advance(reIntegrator& integrator, reFloat dt) override;
  
  // getters for material properties
  reFloat mass() const override;
  const re::mat3& inertia() const override;
  reFloat density() const override;
  
  // setters for material properties
  void setMass(reFloat mass) override;
  void setDensity(reFloat density) override;
  
  RE_ENT_CHAINABLE_METHODS(reRigidBody)
  
protected:
  /** The reRigidBody's mass */
  reFloat _mass;
  /** The reRigidBody's inertia tensor */
  re::mat3 _inertia;
  
  void updateInertia();
};

inline reRigidBody::reRigidBody(reShape* shape) : reSolid(shape), _mass(1.0), _inertia() {
  updateInertia();
}

inline reRigidBody::~reRigidBody() {
  // do nothing
}

inline reEnt::Type reRigidBody::type() const {
  return RIGID;
}

inline void reRigidBody::advance(reIntegrator& op, reFloat dt) {
  op.integrate(_pos, _vel, dt);
  op.integrate(_quat, _angVel, dt);
  _quat = re::normalize(_quat);
}

inline reFloat reRigidBody::mass() const {
  return _mass;
}

inline const re::mat3& reRigidBody::inertia() const {
  return _inertia;
}

inline reFloat reRigidBody::density() const {
  return _mass / _shape->volume();
}

inline void reRigidBody::setMass(reFloat mass) {
  _mass = mass;
  updateInertia();
}

inline void reRigidBody::setDensity(reFloat density) {
  _mass = density / _shape->volume();
  updateInertia();
}

inline void reRigidBody::updateInertia() {
  _inertia = _shape->computeInertia() * _mass;
}

#endif
