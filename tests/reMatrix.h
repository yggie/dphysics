#include "react/Utilities/reMatrix.h"
#include <gtest/gtest.h>

TEST(MatrixTest, Identity) {
  reMatrix m(1.0);
  reMatrix res = m - m*m*m*m;
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      EXPECT_FLOAT_EQ(res[i][j], 0.0);
    }
  }
}

TEST(MatrixTest, Scaling) {
  reMatrix m(1.0), n(3.0), k(5.0);
  reMatrix res = m*n*k;
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      if (i == j) {
        EXPECT_FLOAT_EQ(res[i][j], 15.0);
      } else {
        EXPECT_FLOAT_EQ(res[i][j], 0.0);
      }
    }
  }
}

TEST(MatrixTest, MatrixMultiplication) {
  reMatrix m(1.0);
  reMatrix n(
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );
  
  reMatrix p(
    3, 5, 7,
    6, 9, 1,
    4, 3, 2
  );
  
  reMatrix nm = n*m;
  reMatrix mn = m*n;
  reMatrix pm = p*m;
  
  for (reUInt i = 0; i < 3; i++) {
    for (reUInt j = 0; j < 3; j++) {
      EXPECT_FLOAT_EQ(nm[i][j], n[i][j]);
      EXPECT_FLOAT_EQ(mn[i][j], n[i][j]);
      EXPECT_FLOAT_EQ(pm[i][j], p[i][j]);
    }
  }
}

