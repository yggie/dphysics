#include "helpers.h"

TEST(reTransformTest, IsIdentityOnInit) {
  reTransform transform;
  
  // should be equal to identity
  ASSERT_TRUE(re::similar(transform.m, re::mat3(1.0))) <<
    "should have no initial rotation or scaling";
  ASSERT_TRUE(re::similar(transform.v, re::vec3(0.0, 0.0, 0.0))) <<
    "should have no initial translation";
}

TEST(reTransformTest, Concatenation) {
}

