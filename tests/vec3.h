#include "helpers.h"

TEST(VectorTest, IsZeroOnInit) {
  re::vec3 v;
  
  ASSERT_VEC_EQ(v, re::vec3(0.0, 0.0, 0.0));
}

TEST(VectorTest, Normalize) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    EXPECT_FLOAT_EQ(re::length(re::normalize(re::vec3::random())), 1.0);
  }
}

TEST(VectorTest, AdditionAndSubtraction) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat vx = reRandom();
    const reFloat vy = reRandom();
    const reFloat vz = reRandom();
    
    const reFloat wx = reRandom();
    const reFloat wy = reRandom();
    const reFloat wz = reRandom();
    
    const re::vec3 v(vx, vy, vz);
    const re::vec3 w(wx, wy, wz);
    
    // addition
    ASSERT_VEC_EQ(v + w, re::vec3(vx + wx, vy + wy, vz + wz));
    // subtraction
    ASSERT_VEC_EQ(v - w, re::vec3(vx - wx, vy - wy, vz - wz));
    
    // ensure consistency
    ASSERT_VEC_EQ(v + w, re::vec3(v) += w);
    ASSERT_VEC_EQ(v - w, re::vec3(v) -= w);
  }
}

TEST(VectorTest, ScalarMultiplicationAndDivision) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const reFloat vx = reRandom();
    const reFloat vy = reRandom();
    const reFloat vz = reRandom();
    const reFloat s = reRandom();
    
    const re::vec3 v(vx, vy, vz);
    
    // multiplication
    ASSERT_VEC_EQ(v * s, re::vec3(vx*s, vy*s, vz*s));
    // division
    ASSERT_VEC_EQ(v / s, re::vec3(vx/s, vy/s, vz/s));
    
    // ensure consistency
    ASSERT_VEC_EQ(v * s, re::vec3(v) *= s);
    ASSERT_VEC_EQ(v / s, re::vec3(v) /= s);
  }
}

TEST(VectorTest, DotProduct) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 a = re::vec3::random();
    const re::vec3 b = re::vec3::random();
    
    ASSERT_FLOAT_EQ(re::dot(a, b), a.x*b.x + a.y*b.y + a.z*b.z);
  }
}

TEST(VectorTest, CrossProduct) {
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const re::vec3 a = re::vec3::random();
    const re::vec3 b = re::vec3::random();
    
    // using properties of a plane
    ASSERT_LE(reAbs(re::dot(re::cross(a, b), a - b)), RE_FP_TOLERANCE);
  }
}

