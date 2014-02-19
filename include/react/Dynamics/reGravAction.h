/**
 * @file
 * Contains the definition of the reGravAction class
 */
#ifndef RE_GRAV_ACTION_H
#define RE_GRAV_ACTION_H

#include "react/Entities/Entity.h"
#include "react/Dynamics/reInteraction.h"

/**
 * @ingroup dynamics
 * Simulates gravity between entities
 */

class reGravAction : public reInteraction {
public:
  void solve(re::Entity& A, re::Entity& B) override;
};

namespace {
  const reFloat G = 0.0001;
}

inline void reGravAction::solve(re::Entity& A, re::Entity& B) {
  const re::vec3 diff = A.center() - B.center();
  const re::vec3 f = (G*A.mass()*B.mass() / re::lengthSq(diff)) * re::normalize(diff);
  A.addImpulse(-f);
  B.addImpulse(f);
}

#endif
