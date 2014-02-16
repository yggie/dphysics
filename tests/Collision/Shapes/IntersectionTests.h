#include "helpers.h"

#include "react/Collision/Shapes/shapes.h"

TEST(IntersectionTests, Sphere_Sphere_test) {
  const re::Sphere s(1.0);

  re::Transform m;
  re::Intersect result;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    m.v = re::vec3::rand(4.0);

    const bool intersects = re::intersects(s, m, s, IDEN_TRANS, result);
    ASSERT_EQ(re::length(m.v) < s.radius()*2, intersects) <<
      "should return true if the distance between the two spheres is less than the sum of the individual radius";

    if (intersects) {
    ASSERT_LE(re::abs(re::dot(result.normal, m.v) - re::length(m.v)), RE_FP_TOLERANCE) <<
      "should have the intersection normal parallel to the position of the displaced sphere";

    ASSERT_LE(re::length(result.point * 2 - m.v), RE_FP_TOLERANCE) <<
      "should return the intersection point as the midpoint of the two spheres";
    }

    m.rotate(re::randf(6.0), re::vec3::rand(100.0));

    ASSERT_EQ(intersects, re::intersects(s, m, s, IDEN_TRANS, result)) <<
      "should be independent of rotation";

    m.m = IDEN_MAT;
  }
}

TEST(IntersectionTests, Plane_Sphere_test) {
  const re::Sphere s(1.0);

  re::Transform transform;
  re::Intersect result;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    const re::Plane plane(re::vec3::rand(100.0), re::randf(2.0));
    transform.v = re::vec3::rand(2.0);

    const bool intersects = re::intersects(s, transform, plane, IDEN_TRANS, result);

    const reFloat dist = re::dot(transform.v, plane.normal()) - plane.offset();
    ASSERT_EQ(intersects, re::abs(dist) < RE_FP_TOLERANCE + s.radius()) <<
      "should return true if the distance between the plane surface and the sphere center is less than the radius" << i;

    if (intersects) {
      ASSERT_FLOAT_EQ(re::abs(re::dot(plane.normal(), result.normal)), 1.0) <<
        "should return an intersection normal parallel to the plane face normal";
    }
  }
}
