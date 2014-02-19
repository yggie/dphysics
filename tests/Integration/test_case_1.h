#include "helpers.h"

#include "react/react.h"

TEST(Integration, TestCase_1) {
  reWorld world;

  const re::vec3 vel(1.0, 0.0, 0.0);
  re::Rigid& sphere = world.build().Rigid(re::Sphere(1.0)).at(-2.0, 0.0, 0.0).movingAt(vel);
  
  re::Static& plane = world.build().Static(re::Plane(-vel, 0.0));

  for (int i = 0; i < 1000; i++) {
    world.advance(0.05);
  }

  ASSERT_LE(re::dot(vel, sphere.vel()), 0.0) <<
    "should have the sphere moving in the opposite direction";

  ASSERT_LE(re::abs(re::length(sphere.vel()) - re::abs(re::dot(sphere.vel(), -vel))), RE_FP_TOLERANCE) <<
    "should have the sphere velocity parallel to the plane";

  ASSERT_FLOAT_EQ(re::lengthSq(plane.vel()), 0.0) <<
    "should not cause movement in StaticBody instances";
}
