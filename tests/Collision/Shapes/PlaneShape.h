#include "helpers.h"

#include "react/Collision/Shapes/PlaneShape.h"

TEST(PlaneShape, Constructor) {
  const re::vec3 dir(1.0, 0.0, 0.0);
  const reFloat offset = 10.0;
  const re::PlaneShape planes[2] = {
    PlaneShape(dir * 100, re::vec3(offset, 50.0, -130.0)),
    PlaneShape(dir * 30, offset)
  };

  for (int i = 0; i < 2; i++) {
    const re::PlaneShape& plane = planes[i];

    ASSERT_FLOAT_EQ(re::length(plane.normal()), 1.0) <<
      "should have a unit vector as its normal";

    ASSERT_FLOAT_EQ(re::length(plane.normal() - dir), 0.0) <<
      "should be facing the right direction";

    ASSERT_FLOAT_EQ(plane.offset(), offset) <<
      "should have the correct offset";

    ASSERT_TRUE(plane.type() == reShape::PLANE_SHAPE) <<
      "should have the correct type assigned";

    ASSERT_EQ(plane.numVerts(), 0) <<
      "should have no reportable vertex";

    ASSERT_FLOAT_EQ(plane.volume(), 0.0) <<
      "should have zero volume";

    ASSERT_LE(re::length(plane.center() - dir * offset), RE_FP_TOLERANCE) <<
      "should report the center as being the closest point to the origin";
  }
}

TEST(PlaneShape, randomPoint_test) {
  for (unsigned int i = 0; i < NUM_REPEATS; i++) {
    const re::PlaneShape plane(re::vec3::rand(), re::randf(100.0));
    for (unsigned int j = 0; j < NUM_REPEATS; j++) {
      ASSERT_LE(re::abs(re::dot(plane.normal(), plane.randomPoint()) - plane.offset()), RE_FP_TOLERANCE) <<
        "should generate random points on the plane";
    }
  }
}

