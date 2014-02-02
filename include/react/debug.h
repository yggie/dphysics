/**
 * @file
 * Contains useful utility methods such as formatted printing of re::vec3 and
 * re::mat3 values
 */
#ifndef RE_UTILS_H
#define RE_UTILS_H

#include <cstdio>
#include <string>
#include "react/math.h"

inline std::string toString(const re::vec3& v) {
  char buffer[255];
  sprintf(&buffer[0], "( %+6.3f, %+6.3f, %+6.3f )", v[0], v[1], v[2]);
  return &buffer[0];
}

inline std::string toString(const re::quat& q) {
  char buffer[255];
  sprintf(&buffer[0], "{ %+6.3f, %+6.3f, %+6.3f, %+6.3f }", q[0], q[1], q[2], q[3]);
  return &buffer[0];
}

inline void rePrint(const re::vec3& v) {
  printf("  %s\n", toString(v).c_str());
}

inline void rePrint(const re::quat& q) {
  printf("  %s\n", toString(q).c_str());
}

inline void rePrint(const re::mat3& m) {
  for (int i = 0; i < 3; i++) {
    printf(" | %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2]);
  }
}

inline void rePrint(const re::mat4x4& m) {
  for (int i = 0; i < 4; i++) {
    printf(" | %+f, %+f, %+f, %+f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
  }
}

#endif
