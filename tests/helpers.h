#ifndef RE_TESTS_HELPERS_H
#define RE_TESTS_HELPERS_H

#include "react/math.h"
#include "react/debug.h"

#include <gtest/gtest.h>
#include <exception>
#include <cstdio>

using namespace re;

struct ValueMismatchException : public std::exception {
  virtual const char* what() const throw() {
    return "Expected match but found mismatch";
  }
} val_mismatch;

struct ValueMatchException : public std::exception {
  virtual const char* what() const throw() {
    return "Expected mismatch but found match";
  }
} val_match;

const reUInt NUM_REPEATS = 50;
const reUInt NUM_SAMPLES = 1e4;

const re::vec3 ZERO_VEC = re::vec3(0.0, 0.0, 0.0);
const re::mat3 IDEN_MAT = re::mat3(1.0);

//#define ASSERT_VEC_EQ(a, b) assertEq(a, b, __FILE__, __LINE__, #a, #b)
//#define ASSERT_QUAT_EQ(q, p) assertEq(q, p, __FILE__, __LINE__, #q, #p)
//#define ASSERT_MAT_EQ(m, n) assertEq(m, n, __FILE__, __LINE__, #m, #n)
//#define ASSERT_MAT_NEQ(m, n) assertNEq(m, n, __FILE__, __LINE__, #m, #n)

#define TEST_STD_PRINT(a, b) \
      printf("%s:%d: Failure\nLHS Expression : %s\nLHS Value      : %s\n", f, l, LHS, toString(a).c_str()), \
      printf("RHS Expression : %s\nRHS Value      : %s\n", RHS, toString(b).c_str())

void assertEq(const vec& a, const vec& b, const char* f, reUInt l, const char* LHS, const char* RHS) {
  for (reUInt i = 0; i < 3; i++) {
    if (re::abs(a[i] - b[i]) > RE_FP_TOLERANCE) {
      TEST_STD_PRINT(a, b);
      throw val_mismatch;
    }
  }
}

void assertEq(const quat& q, const quat& p, const char* f, reUInt l, const char* LHS, const char* RHS) {
  for (reUInt i = 0; i < 3; i++) {
    if (re::abs(q[i] - p[i]) > RE_FP_TOLERANCE) {
      TEST_STD_PRINT(q, p);
      throw val_mismatch;
    }
  }
}

inline void assertEq(const re::mat3& m, const re::mat3& n, const char* f, reUInt l, const char* LHS, const char* RHS) {
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      if (re::abs(m[i][j] - n[i][j]) > RE_FP_TOLERANCE) {
        printf("%s:%d: Failure\nLHS Expression: %s\nLHS Value:\n", f, l, LHS);
        rePrint(m);
        printf("RHS Expression: %s\nRHS Value:\n", RHS);
        rePrint(n);
        throw val_mismatch;
      }
    }
  }
}

inline void assertNEq(const re::mat3& m, const re::mat3& n, const char* f, reUInt l, const char* LHS, const char* RHS) {
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      if (re::abs(m[i][j] - n[i][j]) > RE_FP_TOLERANCE) {
        return;
      }
    }
  }
  printf("%s:%d: Failure\nLHS Expression: %s\nLHS Value:\n", f, l, LHS);
  rePrint(m);
  printf("RHS Expression: %s\nRHS Value:\n", RHS);
  rePrint(n);
  throw val_match;
}

#endif
