/**
 * @file
 * Contains the definition of the re::StaticBody class
 */
#ifndef RE_STATIC_BODY_H
#define RE_STATIC_BODY_H

#include "react/Entities/reSolid.h"

namespace re {
  class StaticBody : public reSolid {
  public:
    StaticBody(reShape* shape);
    StaticBody(const StaticBody&) = delete;
    virtual ~StaticBody();

    StaticBody& operator=(const StaticBody&) = delete;

    Type type() const override;

    void advance(reIntegrator& integrator, reFloat dt) override;
    void addImpulse(const re::vec3& impulse) override;

    reFloat mass() const override;
    reFloat massInv() const override;
    const re::mat3 inertia() const override;
    const re::mat3& inertiaInv() const override;
    reFloat density() const override;

    void setMass(reFloat mass) override;
    void setDensity(reFloat density) override;

    RE_ENT_CHAINABLE_METHODS(StaticBody)
  private:
    const re::mat3 _massInv; 
  };

  inline StaticBody::StaticBody(reShape* shape) : reSolid(shape), _massInv(0.0) {
    // do nothing
  }

  inline StaticBody::~StaticBody() {
    // do nothing
  }

  inline reEnt::Type StaticBody::type() const {
    return STATIC;
  }

  inline void StaticBody::advance(reIntegrator&, reFloat) {
    // do nothing
  }

  inline void StaticBody::addImpulse(const re::vec3&) {
    // do nothing
  }

  inline reFloat StaticBody::mass() const {
    return RE_INFINITY;
  }
  
  inline reFloat StaticBody::massInv() const {
    return 0.0;
  }

  inline const re::mat3 StaticBody::inertia() const {
    return re::mat3(RE_INFINITY);
  }

  inline const re::mat3& StaticBody::inertiaInv() const {
    return _massInv;
  }

  inline reFloat StaticBody::density() const {
    return RE_INFINITY;
  }

  inline void StaticBody::setMass(reFloat) {
    // do nothing
  }

  inline void StaticBody::setDensity(reFloat) {
    // do nothing
  }
};

#endif
