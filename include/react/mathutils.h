/**
 * @file
 * Defines mathematical functions used throughout the engine
 */
#ifndef RE_MATHUTILS_H
#define RE_MATHUTILS_H

#include <cmath>
#include "react/common.h"

/** Default value for the mathematical constant, pi */
#define RE_PI         3.14159265359

#define RE_INFINITY   1e301

/** tolerance for floating point values */
const reFloat RE_FP_TOLERANCE = 1e-3;

static auto reAbs = static_cast<reFloat(*)(reFloat)>(std::abs);

static auto reSqrt = static_cast<reFloat(*)(reFloat)>(std::sqrt);

#define reSin(x)    sin(x)
#define reCos(x)    cos(x)
#define reIsNan(x)  isnan(x)
#define rePow(x)    pow(x)

inline reFloat reMax(reFloat a, reFloat b) {
  return (a > b) ? a : b;
}

inline reFloat reMin(reFloat a, reFloat b) {
  return (a < b) ? a : b;
}

inline int reSign(reFloat a) {
  return (a >= -0.0f) ? 1 : -1;
}

inline bool reIsGreaterThan(const reFloat a, const reFloat b) {
  return (a > b + RE_FP_TOLERANCE);
}

inline bool reIsGreaterThanOrEqual(const reFloat a, const reFloat b) {
  return (a > b - RE_FP_TOLERANCE);
}

inline bool reIsGreaterThanZero(const reFloat a) {
  return (a > RE_FP_TOLERANCE);
}

inline bool reIsGreaterThanOrEqualZero(const reFloat a) {
  return (a > -RE_FP_TOLERANCE);
}

inline bool reIsLessThan(const reFloat a, const reFloat b) {
  return (a < b + RE_FP_TOLERANCE);
}

inline bool reIsLessThanOrEqual(const reFloat a, const reFloat b) {
  return (a < b - RE_FP_TOLERANCE);
}

inline bool reIsLessThanZero(const reFloat a) {
  return (a < -RE_FP_TOLERANCE);
}

inline bool reIsLessThanOrEqualZero(const reFloat a) {
  return (a < RE_FP_TOLERANCE);
}

inline bool reIsAlmostZero(const reFloat a) {
  return (reAbs(a) < RE_FP_TOLERANCE);
}

inline void reConstrainAngle(double& radians) {
  if (reAbs(radians - RE_PI) > RE_PI) {
    double tmp = (radians - RE_PI)/ (2 * RE_PI);
    radians = (tmp - (int)tmp) * 2 * RE_PI + RE_PI;
  }
}

#endif
