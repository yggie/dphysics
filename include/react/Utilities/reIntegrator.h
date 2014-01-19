/**
 * @file
 * Contains the definition of the reIntegrator utility class
 */
#ifndef RE_INTEGRATOR_H
#define RE_INTEGRATOR_H

#include "math.h"

/**
 * @ingroup utilities
 * Defines an integrator used in the time update step to advance entity states
 */

struct reIntegrator {
  virtual void integrate(reQuaternion& o, reVector& w, reFloat dt);
};

inline void reIntegrator::integrate(reQuaternion& o, reVector& w, reFloat dt) {
  o = o + (o * w) * 0.5 * dt;
}

#endif
