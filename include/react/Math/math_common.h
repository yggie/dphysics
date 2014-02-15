/**
 * @file
 * Defines mathematical functions used throughout the engine
 */
#ifndef RE_MATHUTILS_H
#define RE_MATHUTILS_H

#include <cmath>
#include <cstdlib>
#include "react/common.h"

/** Default value for the mathematical constant, pi */
#define RE_PI                   3.14159265359

#define RE_INFINITY             1e301
#define RE_NEGATIVE_INFINITY    -1e301

/** tolerance for floating point values */
const reFloat RE_FP_TOLERANCE = 1e-3;

namespace re {
  inline reFloat abs(reFloat x) {
    return std::abs(x);
  }

  inline reFloat sqrt(reFloat x) {
    return std::sqrt(x);
  }
  
  inline reFloat sq(reFloat x) {
    return x*x;
  }

  inline reFloat sin(reFloat x) {
    return std::sin(x);
  }
  
  inline reFloat cos(reFloat x) {
    return std::cos(x);
  }
  
  inline bool isnan(reFloat x) {
    return std::isnan(x);
  }
  
  inline reFloat pow(reFloat a, reFloat x) {
    return std::pow(a, x);
  }

  inline reFloat randf() {
    return (std::rand() % 10000) / 10000.0;
  }

  inline reFloat randf(reFloat lower, reFloat upper) {
    return (upper - lower) * randf() + lower;
  }
  
  inline reFloat randf(reFloat bound) {
    return randf(bound, bound);
  }
 
  inline reInt randi() {
    return std::rand();
  }

  inline reFloat max(reFloat a, reFloat b) {
    return (a > b) ? a : b;
  }

  inline reFloat min(reFloat a, reFloat b) {
    return (a < b) ? a : b;
  }

  inline reFloat clamp(reFloat a, reFloat min, reFloat max) {
    return re::max(re::min(a, max), min);
  }

  inline reInt sign(reFloat a) {
    return (a >= -0.0f) ? 1 : -1;
  }

  inline void constrainAngle(reFloat& radians) {
    if (re::abs(radians - RE_PI) > RE_PI) {
      reFloat tmp = (radians - RE_PI)/ (2 * RE_PI);
      radians = (tmp - (reInt)tmp) * 2 * RE_PI + RE_PI;
    }
  }
}

#endif
