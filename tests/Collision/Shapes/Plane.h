#include "helpers.h"

#include "react/Collision/Shapes/Plane.h"

TEST(rePlaneTest, CONSTRUCTION) {
  const re::Plane plane(re::vec3(1000.0, 0.0, 0.0), re::vec3(50.0, 130.0, -540.0));

  ASSERT_FLOAT_EQ(re::length(plane.normal()), 1.0) <<
    "should always have the face normal as a unit vector";

  ASSERT_FLOAT_EQ(plane.offset(), 50.0) <<
    "should have the correct offset";

  const re::Plane plane2(re::vec3(0.01, 0.0, 0.0), 50.0);

  ASSERT_FLOAT_EQ(plane.offset(), 50.0) <<
    "should have the correct offset";
}

TEST(rePlaneTest, RANDOM_POINT) {
  const re::vec3 v = re::vec3(0.0, 0.0, 1.0);
  const rePlane plane(v, 50.0);

  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    ASSERT_LE(re::abs(re::dot(plane.randomPoint(), v) - 50.0), RE_FP_TOLERANCE) <<
      "should generate random points on the plane";
}
