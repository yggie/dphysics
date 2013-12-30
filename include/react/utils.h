#ifndef RE_UTILS_H
#define RE_UTILS_H

#include <cstdio>
#include "react/vector.h"
#include "react/matrix.h"

namespace re {
  void print(const vec& v) {
    printf(" ( %+f, %+f, %+f )\n", v[0], v[1], v[2]);
  }
  
  void print(const mat& m) {
    for (int i = 0; i < 3; i++) {
      printf(" | %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2]);
    }
  }
}

#endif
