#ifndef D_DEBUG_H
#define D_DEBUG_H

#include <stdio.h>
#include "vec.h"

namespace d {
  void print(const vec& v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
  }
}

#endif
