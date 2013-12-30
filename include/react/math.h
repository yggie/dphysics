#ifndef RE_MATH_H
#define RE_MATH_H

#include <cmath>
#include "react/common.h"

namespace re {
  static auto abs = static_cast<double(*)(double)>(std::abs);
  
  static auto sqrt = static_cast<double(*)(double)>(std::sqrt);
  
  template <class T>
  inline T sq(T a) {
    return a*a;
  }

  template <class T>
  inline T max(T a, T b) {
    return (a > b) ? a : b;
  }

  template <class T>
  inline T min(T a, T b) {
    return (a < b) ? a : b;
  }

  template <class T>
  inline int sign(T a) {
    return (a >= -0.0f) ? 1 : -1;
  }

  inline void constrainAngle(double& radians) {
    if (re::abs(radians - PI) > PI) {
      double tmp = (radians - PI)/ (2 * PI);
      radians = (tmp - (int)tmp) * 2 * PI + PI;
    }
  }
}

#endif
