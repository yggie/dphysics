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
  
  void addImpulse(const re::vec3& impulse) override;
  
  // getters for material properties
  reFloat mass() const override;
  reFloat massInv() const override;
  const re::mat3 inertia() const override;
  const re::mat3& inertiaInv() const override;
  reFloat density() const override;
  
  // setters for material properties
  void setMass(reFloat mass) override;
  void setDensity(reFloat density) override;
  
  RE_ENT_CHAINABLE_METHODS(reRigidBody)
  
protected:
  /** The reRigidBody's mass */
  reFloat _massInv;
  /** The reRigidBody's inertia tensor */
  re::mat3 _inertiaInv;
  re::vec3 _linearImpulse;
  
  void updateInertia();
};

inline reRigidBody::reRigidBody(reShape* shape) : reSolid(shape), _massInv(1.0), _inertiaInv(), _linearImpulse() {
  updateInertia();
}

inline reRigidBody::~reRigidBody() {
  // do nothing
}

inline reEnt::Type reRigidBody::type() const {
  return RIGID;
}

inline void reRigidBody::advance(reIntegrator& op, reFloat dt) {
  _vel += _linearImpulse;
  op.integrate(_pos, _vel, dt);
  op.integrate(_quat, _angVel, dt);
  _linearImpulse.set(0.0, 0.0, 0.0);
}

inline void reRigidBody::addImpulse(const re::vec3& impulse) {
  _linearImpulse += impulse * _massInv;
}

inline reFloat reRigidBody::mass() const {
  return 1.0 / _massInv;
}

inline reFloat reRigidBody::massInv() const {
  return _massInv;
}

inline const re::mat3 reRigidBody::inertia() const {
  return re::inverse(_inertiaInv);
}

inline const re::mat3& reRigidBody::inertiaInv() const {
  return _inertiaInv;
}

inline reFloat reRigidBody::density() const {
  return 1.0 / (_shape->volume() * _massInv);
}

inline void reRigidBody::setMass(reFloat mass) {
  _massInv = 1.0 / mass;
  updateInertia();
}

inline void reRigidBody::setDensity(reFloat density) {
  _massInv = _shape->volume() / density;
  updateInertia();
}

inline void reRigidBody::updateInertia() {
  _inertiaInv = re::inverse(_shape->computeInertia()) / _massInv;
}

#endif
