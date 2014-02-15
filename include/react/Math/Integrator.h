/**
 * @file
 * Contains the definition of the reIntegrator utility class
 */
#ifndef RE_INTEGRATOR_H
#define RE_INTEGRATOR_H

#include "math.h"

namespace re {
  /**
   * @ingroup utilities
   * Defines an integrator used in the time update step to advance entity states
   */

  struct Integrator {
    void integrate(re::vec3& p, re::vec3& v, reFloat dt);
    void integrate(re::quat& o, re::vec3& w, reFloat dt);
  };

  /**
   * Applies the integration scheme to update positional state
   * 
   * @param p The position vector
   * @param v The velocity vector
   * @param dt The time step in user-defined units
   */

  inline void Integrator::integrate(re::vec3& p, re::vec3& v, reFloat dt) {
    p += v*dt;
  }

  /**
   * Applies the integration scheme to update rotational state
   * 
   * @param o The orientation expressed as a quaternion
   * @param w The angular velocity
   * @param dt The time step in user-defined units
   */

  inline void Integrator::integrate(re::quat& o, re::vec3& w, reFloat dt) {
    o += (w * o) * 0.5 * dt;
    o = re::normalize(o);
  }
}

#endif
