/**
 * @file
 * Contains the definition of the re::Static class
 */
#ifndef RE_STATIC_H
#define RE_STATIC_H

#include "react/Entities/Entity.h"

namespace re {
  class Static : public Entity {
  public:
    Static(reShape& shape);
    Static(const Static&) = delete;
    virtual ~Static();

    Static& operator=(const Static&) = delete;

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

    void advance(re::Integrator& integrator, reFloat dt) override;
    void addImpulse(const re::vec3& impulse) override;

    reFloat mass() const override;
    reFloat massInv() const override;
    const re::mat3 inertia() const override;
    const re::mat3& inertiaInv() const override;
    reFloat density() const override;
    reFloat restitution() const override;
    reFloat friction() const override;
    reFloat resistance() const override;

    void setMass(reFloat mass) override;
    void setDensity(reFloat density) override;
    void setRestitution(reFloat restitution) override;
    void setFriction(reFloat friction) override;
    void setResistance(reFloat resistance) override;

    EXTENDS_ENTITY(Static)
  private:
    re::quat _orient;
    reFloat _restitution;
    reFloat _friction;
    reFloat _resistance;
  };

  inline Static::Static(reShape& shape) : Entity(shape), _orient(), _restitution(0.6), _friction(0.3), _resistance(0.01) {
    // do nothing
  }

  inline Static::~Static() {
    // do nothing
  }

  inline Entity::Type Static::type() const {
    return STATIC;
  }

  inline const re::vec3& Static::vel() const {
    return re::vec3::ZERO;
  }

  inline const re::mat3 Static::rot() const {
    return re::toMat(_orient);
  }

  inline const re::quat& Static::orient() const {
    return _orient;
  }

  inline const re::vec3& Static::angVel() const {
    return re::vec3::ZERO;
  }

  inline void Static::setVel(const re::vec3&) {
    // do nothing
  }

  inline void Static::setVel(reFloat, reFloat, reFloat) {
    // do nothing
  }

  inline void Static::setAngVel(const re::vec3&) {
    // do nothing
  }

  inline void Static::setAngVel(reFloat, reFloat, reFloat) {
    // do nothing
  }

  inline void Static::setFacing(const re::vec3& dir, const re::vec3& up) {
    _orient = re::toQuat(re::orientY(dir, up));
  }

  inline void Static::advance(re::Integrator&, reFloat) {
    // do nothing
  }

  inline void Static::addImpulse(const re::vec3&) {
    // do nothing
  }

  inline reFloat Static::mass() const {
    return RE_INFINITY;
  }
  
  inline reFloat Static::massInv() const {
    return 0.0;
  }

  inline const re::mat3 Static::inertia() const {
    return re::mat3(RE_INFINITY);
  }

  inline const re::mat3& Static::inertiaInv() const {
    return re::mat3::ZERO;
  }

  inline reFloat Static::density() const {
    return RE_INFINITY;
  }

  inline reFloat Static::restitution() const {
    return _restitution;
  }

  inline reFloat Static::friction() const {
    return _friction;
  }

  inline reFloat Static::resistance() const {
    return _resistance;
  }

  inline void Static::setMass(reFloat) {
    // do nothing
  }

  inline void Static::setDensity(reFloat) {
    // do nothing
  }

  inline void Static::setRestitution(reFloat) {
    // do nothing
  }

  inline void Static::setFriction(reFloat) {
    // do nothing
  }

  inline void Static::setResistance(reFloat) {
    // do nothing
  }
};

#endif
