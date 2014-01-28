#include "helpers.h"

using namespace re;

TEST(MatrixTest, Identity) {
  mat3x3 eye(1.0);
  // multiplication with the identity matrix should leave the matrix unchanged
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 r = mat3x3::random();
    ASSERT_MAT_EQ(eye*r*eye, r);
  }
}

TEST(MatrixTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const mat3x3 a = mat3x3::random();
    const mat3x3 b = mat3x3::random();
    
    // ensure consistency
    ASSERT_MAT_EQ(a + b, mat3x3(a) += b);
    ASSERT_MAT_EQ(a - b, mat3x3(a) -= b);
  }
}

TEST(MatrixTest, Scaling) {
  // scaling matrices should work
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat s = 10.0 * (reRandom() - 0.5);
    const reFloat v = 10.0 * (reRandom() - 0.5);
    ASSERT_MAT_EQ(mat3x3(s)*mat3x3(v), mat3x3(s*v));
  }
}

TEST(MatrixTest, MatrixMultiplication) {
  // ensures no pure diagonal matrices are created
  mat3x3 offset(
    1, 0, 1,
    0, 0, 0,
    0, 1, 0
  );
  mat3x3 a = mat3x3::random() + offset;
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 r = mat3x3::random() + offset;
    // assert non-commutative multiplication
    ASSERT_MAT_NEQ(a*r, r*a);
    // assert distributive multiplication
    ASSERT_MAT_EQ(offset*(a + r), offset*a + offset*r);
    // ensure consistency
    ASSERT_MAT_EQ(a * r, mat3x3(a) *= r);
  }
}

TEST(MatrixTest, Determinant) {
  // determinant should be equal to the product of elements for diagonal matrix
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat s = 100.0 * (reRandom() - 0.5);
    const mat3x3 m(s);
    ASSERT_FLOAT_EQ(determinant(m), s*s*s);
  }
}

TEST(MatrixTest, Transpose) {
  mat3x3 d = mat3x3::random();
  mat3x3 dt = transpose(d);
  // transposition of multiplication
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 o = mat3x3::random();
    ASSERT_MAT_EQ(transpose(o*d), dt*transpose(o));
  }
}

TEST(MatrixTest, Inverse) {
  // matrix multiplication with inverse will return the identity matrix
  mat3x3 eye(1.0);
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 m = mat3x3::random();
    mat3x3 im = inverse(m);
    ASSERT_MAT_EQ(im*m, eye);
  }
}

TEST(MatrixTest, AxisAngle) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::mat3x3 r = re::axisAngle(re::vec3::random(), 1e5*reRandom());
    
    // assert properties of a rotation matrix
    ASSERT_MAT_EQ(re::inverse(r), re::transpose(r));
    ASSERT_FLOAT_EQ(re::determinant(r), 1.0);
  }
}

