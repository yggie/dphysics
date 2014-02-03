#include "helpers.h"

TEST(VectorTest, IsZeroOnInit) {
  re::vec3 v;
  
  ASSERT_TRUE(re::similar(v, re::vec3(0.0, 0.0, 0.0))) <<
    "should be the zero vector on create";
}

TEST(VectorTest, Normalize) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    EXPECT_FLOAT_EQ(re::length(re::vec3::unit()), 1.0);
  }
}

TEST(VectorTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat vx = re::randf();
    const reFloat vy = re::randf();
    const reFloat vz = re::randf();
    
    const reFloat wx = re::randf();
    const reFloat wy = re::randf();
    const reFloat wz = re::randf();
    
    const re::vec3 v(vx, vy, vz);
    const re::vec3 w(wx, wy, wz);
    
    ASSERT_TRUE(re::similar(v + w, re::vec3(vx + wx, vy + wy, vz + wz))) <<
      "should give the same result when each element is added";
    
    ASSERT_TRUE(re::similar(v - w, re::vec3(vx - wx, vy - wy, vz - wz))) <<
      "should give the same result when each element is subtracted";
    
    // ensure consistency
    ASSERT_TRUE(re::similar(v + w, re::vec3(v) += w)) <<
      "should behave similarly for the increment operator";
    ASSERT_TRUE(re::similar(v - w, re::vec3(v) -= w)) <<
      "should behave similarly for the decrement operator";
  }
}

TEST(VectorTest, ScalarMultiplicationAndDivision) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 v = re::vec3::rand();
    const reFloat s = re::randf();
    
    // multiplication
    ASSERT_TRUE(re::similar(v * s, re::vec3(v.x*s, v.y*s, v.z*s))) <<
      "should give the same result when each element is multiplied";
    // division
    ASSERT_TRUE(re::similar(v / s, re::vec3(v.x/s, v.y/s, v.z/s))) <<
      "should give the same result when each element is divided";
    
    // ensure consistency
    ASSERT_TRUE(re::similar(v * s, re::vec3(v) *= s)) <<
      "should behave similarly for the multiplication operator";
    ASSERT_TRUE(re::similar(v / s, re::vec3(v) /= s)) <<
      "should behave similarly for the division decrement operator";
  }
}

TEST(VectorTest, DotProduct) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 a = re::vec3::rand();
    const re::vec3 b = re::vec3::rand();
    
    ASSERT_FLOAT_EQ(re::dot(a, b), a.x*b.x + a.y*b.y + a.z*b.z);
  }
}

TEST(VectorTest, CrossProduct) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 a = re::vec3::rand();
    const re::vec3 b = re::vec3::rand();
    
    // using properties of a plane
    ASSERT_LE(re::abs(re::dot(re::cross(a, b), a - b)), RE_FP_TOLERANCE);
  }
}

