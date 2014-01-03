/**
 * @file
 * Defines mathematical functions used throughout the engine
 */
#ifndef RE_MATHUTILS_H
#define RE_MATHUTILS_H

#include <cmath>
#include "react/common.h"

static auto reAbs = static_cast<double(*)(double)>(std::abs);

static auto reSqrt = static_cast<double(*)(double)>(std::sqrt);

template <class T>
inline T reMax(T a, T b) {
  return (a > b) ? a : b;
}

template <class T>
inline T reMin(T a, T b) {
  return (a < b) ? a : b;
}

template <class T>
inline int reSign(T a) {
  return (a >= -0.0f) ? 1 : -1;
}

inline void reConstrainAngle(double& radians) {
  if (reAbs(radians - RE_PI) > RE_PI) {
    double tmp = (radians - RE_PI)/ (2 * RE_PI);
    radians = (tmp - (int)tmp) * 2 * RE_PI + RE_PI;
  }
}

#endif
