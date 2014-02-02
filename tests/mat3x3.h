#include "helpers.h"

using namespace re;

TEST(MatrixTest, IsIdentityOnInit) {
  mat3x3 m;
  
  mat3x3 eye(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  );
  
  ASSERT_TRUE(re::similar(m, eye)) <<
    "should be identity by default";
}

TEST(MatrixTest, Identity) {
  mat3x3 eye(1.0);
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 r = mat3x3::rand();
    ASSERT_TRUE(re::similar(eye*r*eye, r)) <<
      "should not change when multiplied by the identity matrix";
  }
}

TEST(MatrixTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const mat3x3 A = mat3x3::rand();
    const mat3x3 B = mat3x3::rand();
    
    const mat3x3 AplusB = A + B;
    const mat3x3 AminusB = A - B;
    
    ASSERT_TRUE(re::similar(AplusB, mat3x3(
      A[0][0] + B[0][0], A[0][1] + B[0][1], A[0][2] + B[0][2],
      A[1][0] + B[1][0], A[1][1] + B[1][1], A[1][2] + B[1][2],
      A[2][0] + B[2][0], A[2][1] + B[2][1], A[2][2] + B[2][2]
    ))) << "should be the same as when the elements are added";
    
    ASSERT_TRUE(re::similar(AminusB, mat3x3(
      A[0][0] - B[0][0], A[0][1] - B[0][1], A[0][2] - B[0][2],
      A[1][0] - B[1][0], A[1][1] - B[1][1], A[1][2] - B[1][2],
      A[2][0] - B[2][0], A[2][1] - B[2][1], A[2][2] - B[2][2]
    ))) << "should be the same as when the elements are subtracted";
    
    // ensure consistency
    ASSERT_TRUE(re::similar(AplusB, mat3x3(A) += B)) <<
      "should behave similarly for the increment operator";
    ASSERT_TRUE(re::similar(AminusB, mat3x3(A) -= B)) <<
      "should behave similarly for the decrement operator";
  }
}

TEST(MatrixTest, Scaling) {
  // scaling matrices should work
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat s = re::randf(-5.0, 5.0);
    const reFloat v = re::randf(-5.0, 5.0);
    ASSERT_TRUE(re::similar(mat3x3(s)*mat3x3(v), mat3x3(s*v))) <<
      "should be equivalent to constructing a new scaling matrix from the product of the individual multipliers";
  }
}

TEST(MatrixTest, MatrixMultiplication) {
  // ensures no pure diagonal matrices are created
  mat3x3 offset(
    1, 0, 1,
    0, 0, 0,
    0, 1, 0
  );
  mat3x3 a = mat3x3::rand() + offset;
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 r = mat3x3::rand() + offset;
    ASSERT_FALSE(re::similar(a*r, r*a)) <<
      "should exhibit non-commutative multiplication";
    ASSERT_TRUE(re::similar(offset*(a + r), offset*a + offset*r)) <<
      "should exhibit distributive multiplication";
    ASSERT_TRUE(re::similar(a * r, mat3x3(a) *= r)) <<
      "should behave the same for the multiplication increment operator";
  }
}

TEST(MatrixTest, Determinant) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat s = re::randf(-50.0, 50.0);
    const mat3x3 m(s);
    ASSERT_FLOAT_EQ(determinant(m), s*s*s) <<
      "should be equal to the product of the diagonals for a diagonal matrix";
  }
}

TEST(MatrixTest, Transpose) {
  mat3x3 d = mat3x3::rand();
  mat3x3 dt = transpose(d);
  // transposition of multiplication
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 o = mat3x3::rand();
    
    ASSERT_TRUE(re::similar(transpose(o*d), dt*transpose(o))) <<
      "should adhere to transposition of multiplied arguments";
  }
}

TEST(MatrixTest, Inverse) {
  // matrix multiplication with inverse will return the identity matrix
  mat3x3 eye(1.0);
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    mat3x3 m = mat3x3::rand();
    mat3x3 im = inverse(m);
    ASSERT_TRUE(re::similar(im*m, eye)) <<
      "should return the identity matrix";
  }
}

TEST(MatrixTest, Rotation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::mat3x3 r = re::mat3::rotation(re::randf(-1e5, 1e5), re::vec3::rand());
    
    ASSERT_TRUE(re::similar(re::inverse(r), re::transpose(r))) <<
      "the inverse and transpose should be equivalent for a rotation matrix";

    ASSERT_LE(re::abs(re::determinant(r) - 1.0), RE_FP_TOLERANCE) <<
      "should have determinant equal to 1.0";
  }
}

