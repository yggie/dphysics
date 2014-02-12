#include "helpers.h"

#include "react/Collision/Shapes/Ray.h"

TEST(RayTest, Construction) {
  const re::vec3 origin = re::vec3(1.0, 0.3, -3.0);
  const Ray ray(origin, re::vec3(100.0, 0.0, 0.0));

  ASSERT_FLOAT_EQ(re::length(ray.dir()), 1.0) <<
    "should have its direction normalized";

  ASSERT_LE(re::length(origin - ray.origin()), RE_FP_TOLERANCE) <<
    "should have the correct origin";
}
