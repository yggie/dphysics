#include "helpers.h"

TEST(TransformTest, IsIdentityOnInit) {
  re::Transform transform;
  
  ASSERT_TRUE(re::similar(transform.m, re::mat3(1.0))) <<
    "should have no initial rotation or scaling";

  ASSERT_TRUE(re::similar(transform.v, re::vec3(0.0, 0.0, 0.0))) <<
    "should have no initial translation";
}

TEST(TransformTest, Scaling) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::Transform transform;
    const re::vec3 v = re::vec3::rand(30.0);
    transform.scale(v.x, v.y, v.z);
    
    ASSERT_TRUE(re::similar(transform.m, re::mat3x3(v.x, v.y, v.z))) <<
      "should be equivalent to constructing a 3x3 scaling matrix";

    ASSERT_TRUE(re::similar(transform.v, ZERO_VEC)) <<
      "should have no translation";
    
    const re::vec3 v2 = re::vec3::rand(10.0);
    const re::vec3 vv2(v.x*v2.x, v.y*v2.y, v.z*v2.z);
    
    ASSERT_TRUE(re::similar(transform.applyToPoint(v2), vv2)) <<
      "should scale a point by the correct amount";
    
    ASSERT_TRUE(re::similar(transform.applyToDir(v2), vv2)) <<
      "should scale a direction by the correct amount";
  }
}

TEST(TransformTest, Translating) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::Transform transform;
    const re::vec3 v = re::vec3::rand(30.0);
    transform.translate(v);
    
    ASSERT_TRUE(re::similar(transform.m, IDEN_MAT)) <<
      "should have no rotation or scaling";
    
    ASSERT_TRUE(re::similar(transform.v, v)) <<
      "should have the correct translation component";
    
    const re::vec3 v2 = re::vec3::rand(25.0);
    
    ASSERT_TRUE(re::similar(v2 + v, transform.applyToPoint(v2))) <<
      "should translate a point";
    
    ASSERT_TRUE(re::similar(v2, transform.applyToDir(v2))) <<
      "should leave directions unchanged";
  }
}

TEST(TransformTest, Rotation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::Transform transform1, transform2;
    const re::vec3 axis = re::vec3::rand();
    const reFloat rad = re::randf(-1e5, 1e5);
    transform1.rotate(rad, axis);
    transform2.rotate(rad, re::normalize(axis));
    
    ASSERT_TRUE(re::similar(transform1.applyToDir(axis), axis)) <<
      "should leave the axis unchanged when applied";
    
    ASSERT_TRUE(re::similar(axis, transform1.applyToPoint(axis))) <<
      "should not have a translational component";
    
    ASSERT_TRUE(re::similar(transform1, transform2)) <<
      "should give the same result with an un-normalized axis";
    
    ASSERT_TRUE(re::similar(re::inverse(transform1), re::Transform(re::transpose(transform1.m), re::vec3()))) <<
      "the inverse and transpose should be equivalent";
    
    ASSERT_LE(re::abs(re::det(transform1) - 1.0), RE_FP_TOLERANCE) <<
      "the determinant should equal to one";
  }
}

TEST(TransformTest, Concatenation) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 translation = re::vec3::rand(15.0);
    const re::vec3 scaling = re::vec3::rand(5.0);
    const re::vec3 axis = re::vec3::unit();
    const reFloat rad = re::randf(-1e5, 1e5);
    
    re::Transform T, S, R, X, Y, Z;
    T.translate(translation);
    S.scale(scaling.x, scaling.y, scaling.z);
    R.rotate(rad, axis);
    X.translate(translation).scale(scaling.x, scaling.y, scaling.z).rotate(rad, axis);
    Y.rotate(rad, axis).translate(translation).scale(scaling.x, scaling.y, scaling.z);
    Z.scale(scaling.x, scaling.y, scaling.z).translate(translation).rotate(rad, axis);
    
    ASSERT_TRUE(re::similar(X, R * S * T)) <<
      "should be similar to concatenating the transformation matrices";
    
    ASSERT_FALSE(re::similar(X, T * S * R)) <<
      "should not be commutative";
    
    ASSERT_TRUE(re::similar(Y, S * T * R) && re::similar(Z, R * T * S)) <<
      "should still work for different ordering of transformations";
  }
}

TEST(TransformTest, Inverse) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    re::Transform transform = re::Transform().translate(re::vec3::rand(50.0)).scale(re::vec3::rand(13.0)).rotate(re::randf(-1e5, 1e5), re::vec3::rand());
    
    ASSERT_TRUE(re::similar(re::inverse(transform) * transform, IDEN_MAT3x4)) <<
      "multiplication with inverse should return the identity matrix";
  }
}

