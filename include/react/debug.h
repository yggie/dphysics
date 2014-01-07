/**
 * @file
 * Contains useful utility methods such as formatted printing of reVector and
 * reMatrix values
 */
#ifndef RE_UTILS_H
#define RE_UTILS_H

#include <cstdio>
#include "react/math.h"

inline void rePrint(const reVector& v) {
  printf(" ( %+f, %+f, %+f )\n", v[0], v[1], v[2]);
}

inline void rePrint(const reMatrix& m) {
  for (int i = 0; i < 3; i++) {
    printf(" | %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2]);
  }
}

inline void rePrint(const reTMatrix& m) {
  for (int i = 0; i < 4; i++) {
    printf(" | %+f, %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }
}

#endif
