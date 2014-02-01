#include "helpers.h"

TEST(reTransformTest, IsIdentityOnInit) {
  reTransform transform;
  
  ASSERT_TRUE(re::similar(transform.m, re::mat3(1.0))) <<
    "should have no initial rotation or scaling";

  ASSERT_TRUE(re::similar(transform.v, re::vec3(0.0, 0.0, 0.0))) <<
    "should have no initial translation";
}

TEST(reTransformTest, Scaling) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    reTransform transform;
    const re::vec3 v = re::vec3::rand(30.0);
    transform.scale(v.x, v.y, v.z);
    
    ASSERT_TRUE(re::similar(transform.m, re::mat3x3(v.x, v.y, v.z))) <<
      "should be equivalent to constructing a 3x3 scaling matrix";

    ASSERT_TRUE(re::similar(transform.v, ZERO_VEC)) <<
      "should have no translation";
  }
}

TEST(reTransformTest, Translating) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    reTransform transform;
    const re::vec3 v = re::vec3::rand(30.0);
    transform.translate(v);
    
    ASSERT_TRUE(re::similar(transform.m, IDEN_MAT)) <<
      "should have no rotation or scaling";
    
    ASSERT_TRUE(re::similar(transform.v, v)) <<
      "should have the correct translation component";
  }
}

TEST(reTransformTest, Rotation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    reTransform transform1, transform2;
    const re::vec3 axis = re::vec3::rand();
    const reFloat rad = re::randf(-1e5, 1e5);
    transform1.rotate(rad, axis);
    transform2.rotate(rad, re::normalize(axis));
    
    ASSERT_TRUE(re::similar(transform1.multDir(axis), axis)) <<
      "should leave the axis unchanged when applied";
    
    ASSERT_TRUE(re::similar(transform1, transform2)) <<
      "should give the same result with an un-normalized axis";
  }
}

TEST(reTransformTest, Concatenation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 translation = re::vec3::rand(15.0);
    const re::vec3 scaling = re::vec3::rand(5.0);
    const re::vec3 axis = re::vec3::unit();
    const reFloat rad = re::randf(-1e5, 1e5);
    
    reTransform A, B, C, T;
    A.translate(translation);
    B.scale(scaling.x, scaling.y, scaling.z);
    C.rotate(rad, axis);
    T.translate(translation).scale(scaling.x, scaling.y, scaling.z).rotate(rad, axis);
    
    ASSERT_TRUE(re::similar(T, C * B * A)) <<
      "should be similar to concatenating the transformation matrices";
    
    ASSERT_FALSE(re::similar(T, A * B * C)) <<
      "should not be commutative";
  }
}

