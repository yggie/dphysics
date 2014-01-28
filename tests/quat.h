#include "helpers.h"

using namespace re;

TEST(QuaternionTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const quat q = quat::random();
    const quat p = quat::random();
    
    // addition
    ASSERT_QUAT_EQ(p + q, quat(p[0] + q[0], p[1] + q[1], p[2] + q[2], p[3] + q[3]));
    // subtraction
    ASSERT_QUAT_EQ(p - q, quat(p[0] - q[0], p[1] - q[1], p[2] - q[2], p[3] - q[3]));
    
    // ensure consistency
    ASSERT_QUAT_EQ(p + q, quat(q) += p);
    ASSERT_QUAT_EQ(p - q, quat(p) -= q);
  }
}

TEST(QuaternionTest, AxisAngle) {
  quat p = re::axisAngleQ(re::vec3(1, 1, 1), 0.0);
  
  const reFloat c = reCos(1.0);
  const reFloat s = reSin(1.0);
  
  ASSERT_QUAT_EQ(p, quat(1.0, 0.0, 0.0, 0.0));
  
  quat g = re::axisAngleQ(re::vec3(1, 0, 0), 2.0);
  
  ASSERT_QUAT_EQ(g, quat(c, s, 0.0, 0.0));
  
  quat b = re::axisAngleQ(re::vec3(0, 1, 0), 2.0);
  
  ASSERT_QUAT_EQ(b, quat(c, 0.0, s, 0.0));
  
  for (reUInt i = 0; i < 100; i++) {
    quat q = re::axisAngleQ(re::vec3::random(), 1e5*reRandom());
    ASSERT_FLOAT_EQ(re::length(q), 1.0);
  }
}

TEST(QuaternionTest, QuaternionMultiplication) {
  quat w(1, 0, 0, 0);
  quat x(0, 1, 0, 0);
  quat y(0, 0, 1, 0);
  quat z(0, 0, 0, 1);
  
  quat ww = w*w;
  quat wx = w*x;
  quat wy = w*y;
  quat wz = w*z;
  quat xx = x*x;
  quat yy = y*y;
  quat zz = z*z;
  
  for (reUInt i = 0; i < 4; i++) {
    EXPECT_FLOAT_EQ(ww[i], w[i]);
    EXPECT_FLOAT_EQ(wx[i], x[i]);
    EXPECT_FLOAT_EQ(wy[i], y[i]);
    EXPECT_FLOAT_EQ(wz[i], z[i]);
    EXPECT_FLOAT_EQ(xx[i], -w[i]);
    EXPECT_FLOAT_EQ(yy[i], -w[i]);
    EXPECT_FLOAT_EQ(zz[i], -w[i]);
  }
}

TEST(QuaternionTest, ScalarMultiplicationAndDivision) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::quat q = re::quat::random();
    const reFloat s = reRandom() + 3.0;
    
    // multiplication
    ASSERT_QUAT_EQ(q * s, re::quat(q.r*s, q.i*s, q.j*s, q.k*s));
    // division
    ASSERT_QUAT_EQ(q / s, re::quat(q.r/s, q.i/s, q.j/s, q.k/s));
    
    // ensure consistency
    ASSERT_QUAT_EQ(q * s, re::quat(q) *= s);
    ASSERT_QUAT_EQ(q / s, re::quat(q) /= s);
  }
}

TEST(QuaternionTest, MatrixConversionTest) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat3 m = re::axisAngle(re::vec3::random(), 1e5*reRandom());
    quat q = re::toQuat(m);
    re::mat3 n = re::toMat(q);
    ASSERT_MAT_EQ(m, n);
  }
}

