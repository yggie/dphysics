#include "react/Utilities/reVector.h"
#include <gtest/gtest.h>

TEST(VectorTest, Normalize) {
  reVector vect(1.0, 1.0, 1.0);
  vect.normalize();
  EXPECT_LE(reAbs(vect.length() - 1.0), RE_FP_TOLERANCE);
}

