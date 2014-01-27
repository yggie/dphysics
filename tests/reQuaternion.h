#include "helpers.h"

TEST(QuaternionTest, Addition) {
  reQuaternion q(0, 1, 2, 3), p(3, 2, 1, 0);
  reQuaternion m = q + p;
  for (reUInt i = 0; i < 4; i++) {
    EXPECT_FLOAT_EQ(m[i], 3.0);
  }
}

TEST(QuaternionTest, AxisAngle) {
  reQuaternion p(0.0, reVector(1, 1, 1));
  
  const reFloat c = reCos(1.0);
  const reFloat s = reSin(1.0);
  
  ASSERT_QUAT_EQ(p, reQuaternion(1.0, 0.0, 0.0, 0.0));
  
  reQuaternion g(2.0, reVector(1, 0, 0));
  
  ASSERT_QUAT_EQ(g, reQuaternion(c, s, 0.0, 0.0));
  
  reQuaternion b(2.0, reVector(0, 1, 0));
  
  ASSERT_QUAT_EQ(b, reQuaternion(c, 0.0, s, 0.0));
  
  for (reUInt i = 0; i < 100; i++) {
    reQuaternion q(1e5*reRandom(), reVector::random());
    ASSERT_FLOAT_EQ(q.length(), 1.0);
  }
}

TEST(QuaternionTest, QuaternionMultiplication) {
  reQuaternion w(1, 0, 0, 0);
  reQuaternion x(0, 1, 0, 0);
  reQuaternion y(0, 0, 1, 0);
  reQuaternion z(0, 0, 0, 1);
  
  reQuaternion ww = w*w;
  reQuaternion wx = w*x;
  reQuaternion wy = w*y;
  reQuaternion wz = w*z;
  reQuaternion xx = x*x;
  reQuaternion yy = y*y;
  reQuaternion zz = z*z;
  
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
  reQuaternion w(1, 2, 3, 4);
  reQuaternion s = w * 0.5;
  
  for (reUInt i = 0; i < 4; i++) {
    ASSERT_FLOAT_EQ(s[i], 0.5*w[i]);
  }
}

TEST(QuaternionTest, MatrixConversionTest) {
  for (reUInt i = 0; i < 50; i++) {
    reQuaternion q(1e5*reRandom(), reVector::random());
    reMatrix m = q.toMatrix();
    q.setFromMatrix(m);
    reMatrix n = q.toMatrix();
    ASSERT_MAT_EQ(m, n);
  }
}

