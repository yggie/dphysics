#ifndef RE_DEBUG_H
#define RE_DEBUG_H

#include <stdio.h>
#include "vector.h"
#include "matrix.h"

namespace re {
  void print(const vec& v) {
    printf(" ( %+f, %+f, %+f )\n", v[0], v[1], v[2]);
  }
  
  void print(const mat4& m) {
    for (int i = 0; i < 4; i++) {
      printf(" | %+f, %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
  }
}

#endif
