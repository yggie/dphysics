#ifndef D_MATH_H
#define D_MATH_H

#ifndef PI
#define PI 3.14159265359
#endif

#include <cmath>
#include "common.h"

namespace d {
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

  inline void constrainAngle(double& angle) {
    if (fabs(angle - PI) > PI) {
      double tmp = (angle - PI)/ (2 * PI);
      angle = (tmp - (int)tmp) * 2 * PI + PI;
    }
  }
}

#endif
