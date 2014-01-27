#ifndef RE_TESTS_HELPERS_H
#define RE_TESTS_HELPERS_H

#include "react/math.h"
#include "react/debug.h"

#include <gtest/gtest.h>
#include <exception>
#include <cstdio>

struct VectorMismathException : public std::exception {
  virtual const char* what() const throw() {
    return "Value mismatch";
  }
} val_mismatch;

#define ASSERT_VEC_EQ(a, b) assertVEq(a, b, __FILE__, __LINE__)
#define ASSERT_QUAT_EQ(q, p) assertQEq(q, p, __FILE__, __LINE__)
#define ASSERT_MAT_EQ(m, n) assertMEq(m, n, __FILE__, __LINE__)

void assertVEq(const reVector& a, const reVector& b, const char* f, reUInt l) {
  for (reUInt i = 0; i < 3; i++) {
    if (reAbs(a[i] - b[i]) > RE_FP_TOLERANCE) {
      printf("%s:%d: Failure\nLHS: %s\n", f, l, toString(a).c_str());
      printf("RHS: %s\n", toString(b).c_str());
      throw val_mismatch;
    }
  }
}

void assertQEq(const reQuaternion& q, const reQuaternion& p, const char* f, reUInt l) {
  for (reUInt i = 0; i < 3; i++) {
    if (reAbs(q[i] - p[i]) > RE_FP_TOLERANCE) {
      printf("%s:%d: Failure\nLHS: %s\n", f, l, toString(q).c_str());
      printf("RHS: %s\n", toString(p).c_str());
      throw val_mismatch;
    }
  }
}

inline void assertMEq(const reMatrix& m, const reMatrix& n, const char* f, reUInt l) {
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      if (reAbs(m[i][j] - n[i][j]) > RE_FP_TOLERANCE) {
        printf("%s:%d: Failure\nLHS:\n", f, l);
        rePrint(m);
        printf("RHS:\n");
        rePrint(n);
        throw val_mismatch;
      }
    }
  }
}

#endif
