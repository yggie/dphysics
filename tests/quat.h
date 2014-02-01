#include "helpers.h"

using namespace re;

TEST(QuaternionTest, HasNoRotationOnInit) {
  quat q;
  
  ASSERT_TRUE(re::similar(q, quat(1.0, 0.0, 0.0, 0.0))) <<
    "should be identity by default";
}

TEST(QuaternionTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const quat q = quat::rand();
    const quat p = quat::rand();
    
    ASSERT_TRUE(re::similar(p + q, quat(p[0] + q[0], p[1] + q[1], p[2] + q[2], p[3] + q[3]))) <<
      "should be the same when adding each element individually";

    ASSERT_TRUE(re::similar(p - q, quat(p[0] - q[0], p[1] - q[1], p[2] - q[2], p[3] - q[3]))) <<
      "should be the same when subtracting each element individually";
    
    ASSERT_TRUE(re::similar(p + q, quat(q) += p)) <<
      "should behave similarly to the increment operator";

    ASSERT_TRUE(re::similar(p - q, quat(p) -= q)) <<
      "should behave similarly to the decrement operator";
  }
}

TEST(QuaternionTest, AxisAngle) {
  quat p = re::axisAngleQ(re::vec3(1, 1, 1), 0.0);
  
  const reFloat c = re::cos(1.0);
  const reFloat s = re::sin(1.0);
  
  ASSERT_TRUE(re::similar(p, quat(1.0, 0.0, 0.0, 0.0))) <<
    "should be equal to the predicted quantity";
  
  quat g = re::axisAngleQ(re::vec3(1, 0, 0), 2.0);
  
  ASSERT_TRUE(re::similar(g, quat(c, s, 0.0, 0.0))) <<
    "should be equal to the predicted quantity";
  
  quat b = re::axisAngleQ(re::vec3(0, 1, 0), 2.0);
  
  ASSERT_TRUE(re::similar(b, quat(c, 0.0, s, 0.0))) <<
    "should be equal to the predicted quantity";
  
  for (reUInt i = 0; i < 100; i++) {
    quat q = re::axisAngleQ(re::vec3::rand(), re::randf(-1e5, 1e5));
    ASSERT_FLOAT_EQ(re::length(q), 1.0) <<
      "should always be a unit quaternion";
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
    const re::quat q = re::quat::rand();
    const reFloat s = re::randf(3.0, 4.0);
    
    ASSERT_TRUE(re::similar(q * s, re::quat(q.r*s, q.i*s, q.j*s, q.k*s))) <<
      "should be equivalent to multiplying individual elements";

    ASSERT_TRUE(re::similar(q / s, re::quat(q.r/s, q.i/s, q.j/s, q.k/s))) <<
      "should be equivalent to dividing individual elements";
    
    ASSERT_TRUE(re::similar(q * s, re::quat(q) *= s)) <<
      "should behave similarly to the multiplication increment operator";

    ASSERT_TRUE(re::similar(q / s, re::quat(q) /= s)) <<
      "should behave similarly to the division decrement operator";
  }
}

TEST(QuaternionTest, MatrixConversionTest) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat3 m = re::axisAngle(re::vec3::rand(), re::randf(-1e5, 1e5));
    quat q = re::toQuat(m);
    re::mat3 n = re::toMat(q);
    ASSERT_TRUE(re::similar(m, n)) <<
      "should return the same matrix after converting back from a quaternion";
  }
}

