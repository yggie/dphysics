#include "helpers.h"

TEST(Matrix4x4Test, Creation) {
  re::mat4x4 m;
  
  ASSERT_TRUE(re::similar(m, IDEN_MAT4x4)) <<
    "should be initialized as the identity matrix";
}

TEST(Matrix4x4Test, Scaling) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat4x4 m;
    const re::vec3 v = re::vec3::rand(30.0);
    m.scale(v.x, v.y, v.z);
    
    ASSERT_TRUE(re::similar(m, re::mat4x4(v.x, v.y, v.z, 1.0))) <<
      "should be equivalent to constructing a 3x3 scaling matrix";

    ASSERT_TRUE(re::similar(re::vec3(m[0][3], m[1][3], m[2][3]), ZERO_VEC)) <<
      "should have no translation";
  }
}

TEST(Matrix4x4Test, Translating) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat4x4 m;
    const re::vec3 v = re::vec3::rand(30.0);
    m.translate(v);
    
    ASSERT_TRUE(re::similar(re::mat3(
      m[0][0], m[0][1], m[0][2],
      m[1][0], m[1][1], m[1][2],
      m[2][0], m[2][1], m[2][2]
    ), IDEN_MAT)) <<
      "should have no rotation or scaling";
    
    ASSERT_TRUE(re::similar(re::vec3(m[0][3], m[1][3], m[2][3]), v)) <<
      "should have the correct translation component";
  }
}

TEST(Matrix4x4Test, Rotation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat4x4 m1, m2;
    const re::vec3 axis = re::vec3::rand();
    const reFloat rad = re::randf(-1e5, 1e5);
    m1.rotate(rad, axis);
    m2.rotate(rad, re::normalize(axis));
    
    ASSERT_TRUE(re::similar(m1.multDir(axis), axis)) <<
      "should leave the axis unchanged when applied";
    
    ASSERT_TRUE(re::similar(m1, m2)) <<
      "should give the same result with an un-normalized axis";
  }
}

