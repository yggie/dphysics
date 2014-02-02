#include "helpers.h"

TEST(Matrix4x4Test, IsIdentityOnInit) {
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
    
    const re::vec3 v2 = re::vec3::rand(10.0);
    const re::vec3 vv2(v.x*v2.x, v.y*v2.y, v.z*v2.z);
    
    ASSERT_TRUE(re::similar(m.multPoint(v2), vv2)) <<
      "should scale a point by the correct amount";
    
    ASSERT_TRUE(re::similar(m.multDir(v2), vv2)) <<
      "should scale a direction by the correct amount";
  }
}

TEST(Matrix4x4Test, Translating) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 v = re::vec3::rand(30.0);
    const re::mat4x4 m = re::mat4().translate(v);
    
    ASSERT_TRUE(re::similar(re::mat3(
      m[0][0], m[0][1], m[0][2],
      m[1][0], m[1][1], m[1][2],
      m[2][0], m[2][1], m[2][2]
    ), IDEN_MAT)) <<
      "should have no rotation or scaling";
    
    ASSERT_TRUE(re::similar(re::vec3(m[0][3], m[1][3], m[2][3]), v)) <<
      "should have the correct translation component";
    
    const re::vec3 v2 = re::vec3::rand(25.0);
    
    ASSERT_TRUE(re::similar(v2 + v, m.multPoint(v2))) <<
      "should translate a point";
    
    ASSERT_TRUE(re::similar(v2, m.multDir(v2))) <<
      "should leave directions unchanged";
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

TEST(Matrix4x4Test, Concatenation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 translation = re::vec3::rand(15.0);
    const re::vec3 scaling = re::vec3::rand(5.0);
    const re::vec3 axis = re::vec3::unit();
    const reFloat rad = re::randf(-1e5, 1e5);
    
    re::mat4x4 T, S, R, X, Y, Z;
    T.translate(translation);
    S.scale(scaling.x, scaling.y, scaling.z);
    R.rotate(rad, axis);
    X.translate(translation).scale(scaling.x, scaling.y, scaling.z).rotate(rad, axis);
    Y.rotate(rad, axis).translate(translation).scale(scaling.x, scaling.y, scaling.z);
    Z.scale(scaling.x, scaling.y, scaling.z).translate(translation).rotate(rad, axis);
    
    ASSERT_TRUE(re::similar(X, R * S * T)) <<
      "should be similar to concatenating the transformation matrices";
    
    ASSERT_FALSE(re::similar(T, T * S * R)) <<
      "should not be commutative";
    
    ASSERT_TRUE(re::similar(Y, S * T * R) && re::similar(Z, R * T * S)) <<
      "should still work for different ordering of transformations";
  }
}

TEST(Matrix4x4Test, Inverse) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::mat4x4 m = re::mat4x4().translate(re::vec3::rand(50.0)).scale(re::vec3::rand(13.0)).rotate(re::randf(-1e5, 1e5), re::vec3::rand());
    
    ASSERT_TRUE(re::similar(re::inverse(m) * m, IDEN_MAT4x4)) <<
      "multiplication with inverse should return the identity matrix";
  }
}

