#include "helpers.h"

#include "react/Collision/Shapes/Plane.h"

TEST(Plane, ConstructorAndProperties_test) {
  const re::vec3 dir(1.0, 0.0, 0.0);
  const reFloat offset = 10.0;

  const char* desc[3] = {
    "origin + direction",
    "origin + offset",
    "copy"
  };

  const re::Plane planes[3] = {
    Plane(dir * 100, re::vec3(offset, 50.0, -130.0)),
    Plane(dir * 30, offset),
    Plane(Plane(dir, offset))
  };

  for (int i = 0; i < 2; i++) {
    const re::Plane& plane = planes[i];

    ASSERT_FLOAT_EQ(re::length(plane.normal()), 1.0) <<
      "should have a unit vector as its normal @" << desc[i];

    ASSERT_FLOAT_EQ(re::length(plane.normal() - dir), 0.0) <<
      "should be facing the right direction @" << desc[i];

    ASSERT_FLOAT_EQ(plane.offset(), offset) <<
      "should have the correct offset @" << desc[i];

    ASSERT_TRUE(plane.type() == reShape::PLANE) <<
      "should have the correct type assigned @" << desc[i];

    ASSERT_EQ(plane.numVerts(), 0) <<
      "should have no reportable vertex @" << desc[i];

    ASSERT_FLOAT_EQ(plane.volume(), 0.0) <<
      "should have zero volume @" << desc[i];

    ASSERT_LE(re::length(plane.center() - dir * offset), RE_FP_TOLERANCE) <<
      "should report the center as being the closest point to the origin @" << desc[i];
  }
}

TEST(Plane, randomPoint_test) {
  for (unsigned int i = 0; i < NUM_REPEATS; i++) {
    const re::Plane plane(re::vec3::rand(), re::randf(100.0));
    for (unsigned int j = 0; j < NUM_REPEATS; j++) {
      ASSERT_LE(re::abs(re::dot(plane.normal(), plane.randomPoint()) - plane.offset()), RE_FP_TOLERANCE) <<
        "should generate random points on the plane";
    }
  }
}

