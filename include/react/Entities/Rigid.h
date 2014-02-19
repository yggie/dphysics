/**
 * @file
 * Contains the definition for the re::Rigid class
 */
#ifndef RE_RIGID_H
#define RE_RIGID_H

#include "react/Entities/Entity.h"

namespace re {

  /**
   * @ingroup entities
   * An entity with a non-deformable shell
   * 
   * @see re::Entity
   */

  class Rigid : public Entity {
  public:
    Rigid(reShape& shape);
    Rigid(const re::Rigid&) = delete;
    virtual ~Rigid();
    
    re::Rigid& operator=(const re::Rigid&) = delete;

    //=====================================================
    //    IDENTIFICATION
    //=====================================================

    Type type() const override;

    //=====================================================
    //    PHYSICAL STATE
    //=====================================================

    // getters
    const re::mat3 rot() const override;
    const re::quat& orient() const override;
    const re::vec3& vel() const override;
    const re::vec3& angVel() const override;

    // setters
    void setVel(const re::vec3& vel) override;
    void setVel(reFloat x, reFloat y, reFloat z) override;
    void setAngVel(const re::vec3&) override;
    void setAngVel(reFloat, reFloat, reFloat) override; 
    void setFacing(const re::vec3& dir, const re::vec3& up = re::vec3(0.0, 0.0, 1.0)) override; 

    // operations
    void advance(re::Integrator& integrator, reFloat dt) override;
    void addImpulse(const re::vec3& impulse) override;

    //=====================================================
    //    PHYSICAL PROPERTIES
    //=====================================================

    // getters
    reFloat mass() const override;
    reFloat massInv() const override;
    const re::mat3 inertia() const override;
    const re::mat3& inertiaInv() const override;
    reFloat density() const override;
    reFloat restitution() const override;
    reFloat friction() const override;
    reFloat resistance() const override;
    
    // setters for material properties
    void setMass(reFloat mass) override;
    void setDensity(reFloat density) override;
    void setRestitution(reFloat restitution) override;
    void setFriction(reFloat friction) override;
    void setResistance(reFloat resistance) override;

    // this macro includes the ability to chain methods
    EXTENDS_ENTITY(Rigid)

  private:
    re::vec3 _vel;
    re::quat _orient;
    re::vec3 _angVel;
    /** The re::Rigid's mass */
    reFloat _massInv;
    /** The re::Rigid's inertia tensor */
    re::mat3 _inertiaInv;
    re::vec3 _linearImpulse;
    reFloat _restitution;
    reFloat _friction;
    reFloat _resistance;

    void updateInertia();
  };

  inline Rigid::Rigid(reShape& shape) : Entity(shape), _vel(0.0), _orient(), _angVel(0.0), _massInv(1.0), _inertiaInv(1.0), _linearImpulse(0.0), _restitution(0.6), _friction(0.3), _resistance(0.01) {
    updateInertia();
  }

  inline Rigid::~Rigid() {
    // do nothing
  }

  inline Entity::Type Rigid::type() const {
    return RIGID;
  }

  inline const re::vec3& Rigid::vel() const {
    return _vel;
  }

  inline const re::mat3 Rigid::rot() const {
    return re::toMat(_orient);
  }

  inline const re::quat& Rigid::orient() const {
    return _orient;
  }

  inline const re::vec3& Rigid::angVel() const {
    return _angVel;
  }

  inline void Rigid::setVel(const re::vec3& vel) {
    _vel = vel;
  }

  inline void Rigid::setVel(reFloat vx, reFloat vy, reFloat vz) {
    _vel.set(vx, vy, vz);
  }

  inline void Rigid::setAngVel(const re::vec3& angVel) {
    _angVel = angVel;
  }

  inline void Rigid::setAngVel(reFloat wx, reFloat wy, reFloat wz) {
    _angVel.set(wx, wy, wz);
  }

  inline void Rigid::setFacing(const re::vec3& dir, const re::vec3& up) {
    _orient = re::toQuat(re::orientY(dir, up));
  }

  inline void Rigid::advance(re::Integrator& op, reFloat dt) {
    _vel += _linearImpulse;
    op.integrate(_pos, _vel, dt);
    op.integrate(_orient, _angVel, dt);
    _linearImpulse.set(0.0, 0.0, 0.0);
  }

  inline void Rigid::addImpulse(const re::vec3& impulse) {
    _linearImpulse += impulse * _massInv;
  }

  inline reFloat Rigid::mass() const {
    return 1.0 / _massInv;
  }

  inline reFloat Rigid::massInv() const {
    return _massInv;
  }

  inline const re::mat3 Rigid::inertia() const {
    return re::inverse(_inertiaInv);
  }

  inline const re::mat3& Rigid::inertiaInv() const {
    return _inertiaInv;
  }

  inline reFloat Rigid::density() const {
    return 1.0 / (_shape.volume() * _massInv);
  }

  inline reFloat Rigid::restitution() const {
    return _restitution;
  }

  inline reFloat Rigid::friction() const {
    return _friction;
  }

  inline reFloat Rigid::resistance() const {
    return _resistance;
  }

  inline void Rigid::setMass(reFloat mass) {
    _massInv = 1.0 / mass;
    updateInertia();
  }

  inline void Rigid::setDensity(reFloat density) {
    _massInv = _shape.volume() / density;
    updateInertia();
  }

  inline void Rigid::setRestitution(reFloat restitution) {
    _restitution = restitution;
  }

  inline void Rigid::setFriction(reFloat friction) {
    _friction = friction;
  }

  inline void Rigid::setResistance(reFloat resistance) {
    _resistance = resistance;
  }

  inline void Rigid::updateInertia() {
    _inertiaInv = re::inverse(_shape.computeInertia()) / _massInv;
  }
}

#endif
