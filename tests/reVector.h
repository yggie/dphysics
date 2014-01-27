#include "helpers.h"

TEST(VectorTest, Normalize) {
  reVector vect(1.0, 1.0, 1.0);
  vect.normalize();
  EXPECT_FLOAT_EQ(vect.length(), 1.0);
}

TEST(VectorTest, Addition) {
  reVector v1(0.0, 0.0, 1.0), v2(1.0, 1.0, 0.0);
  reVector v3 = v1 + v2;
  for (int i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(v3[i], 1.0);
  }
}

TEST(VectorTest, MultiplicationWithScalars) {
  reVector v(1.0, 1.0, 1.0);
  v *= 3.0;
  for (int i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(v[i], 3.0);
  }
}

