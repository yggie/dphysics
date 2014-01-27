/**
 * @file
 * Contains the definition of the reIntegrator utility class
 */
#ifndef RE_INTEGRATOR_H
#define RE_INTEGRATOR_H

#include "math.h"

using namespace re;

/**
 * @ingroup utilities
 * Defines an integrator used in the time update step to advance entity states
 */

struct reIntegrator {
  virtual void integrate(reVector& p, reVector& v, reFloat dt);
  virtual void integrate(quat& o, reVector& w, reFloat dt);
};

/**
 * Applies the integration scheme to update positional state
 * 
 * @param p The position vector
 * @param v The velocity vector
 * @param dt The time step in user-defined units
 */

inline void reIntegrator::integrate(reVector& p, reVector& v, reFloat dt) {
  p += v*dt;
}

/**
 * Applies the integration scheme to update rotational state
 * 
 * @param o The orientation expressed as a quaternion
 * @param w The angular velocity
 * @param dt The time step in user-defined units
 */

inline void reIntegrator::integrate(quat& o, reVector& w, reFloat dt) {
  o += (w * o) * 0.5 * dt;
}

#endif
