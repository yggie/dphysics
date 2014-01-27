#include "helpers.h"

using namespace re;

TEST(QuaternionTest, Addition) {
  quat q(0, 1, 2, 3), p(3, 2, 1, 0);
  quat m = q + p;
  for (reUInt i = 0; i < 4; i++) {
    EXPECT_FLOAT_EQ(m[i], 3.0);
  }
}

TEST(QuaternionTest, AxisAngle) {
  quat p(0.0, reVector(1, 1, 1));
  
  const reFloat c = reCos(1.0);
  const reFloat s = reSin(1.0);
  
  ASSERT_QUAT_EQ(p, quat(1.0, 0.0, 0.0, 0.0));
  
  quat g(2.0, reVector(1, 0, 0));
  
  ASSERT_QUAT_EQ(g, quat(c, s, 0.0, 0.0));
  
  quat b(2.0, reVector(0, 1, 0));
  
  ASSERT_QUAT_EQ(b, quat(c, 0.0, s, 0.0));
  
  for (reUInt i = 0; i < 100; i++) {
    quat q(1e5*reRandom(), reVector::random());
    ASSERT_FLOAT_EQ(q.length(), 1.0);
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

TEST(QuaternionTest, MultiplicationByScalar) {
  quat w(1, 2, 3, 4);
  quat s = w * 0.5;
  
  for (reUInt i = 0; i < 4; i++) {
    ASSERT_FLOAT_EQ(s[i], 0.5*w[i]);
  }
}

TEST(QuaternionTest, MatrixConversionTest) {
  for (reUInt i = 0; i < 50; i++) {
    reMatrix m = re::axisAngle(reVector::random(), 1e5*reRandom());
    quat q(m);
    reMatrix n = q.toMatrix();
    ASSERT_MAT_EQ(m, n);
  }
}

