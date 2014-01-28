#ifndef RE_QUAT_OPS_H
#define RE_QUAT_OPS_H

#include "react/Math/quat.h"

using namespace re;

namespace re {

  inline reFloat lengthSq(const quat& q) {
    return q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k;
  }

  inline reFloat length(const quat& q) {
    return reSqrt(lengthSq(q));
  }

  inline const quat normalize(const quat& q) {
    return quat(q) /= length(q);
  }
}

#endif
