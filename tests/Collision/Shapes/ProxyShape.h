#include "helpers.h"

#include "react/Collision/Shapes/shapes.h"

TEST(ProxyShape, Constructor_test) {
  re::Sphere s(0.1);
  reProxyShape ps(&s);
  
  ASSERT_TRUE(ps.type() == reShape::PROXY) << "should have the correct type";
  
  ASSERT_TRUE(ps.shape() == &s) << "should have the correct base shape";
}

TEST(ProxyShape, randomPoint_test) {
  re::Sphere s(1.0);
  re::Transform m;
  m.scale(3, 2, 5);
  m.rotate(re::randf(0.0, 33.3*RE_PI), re::vec3::rand());

  const re::Transform inv = re::inverse(m);
  const reProxyShape proxy(&s, m);
  for (unsigned int i = 0; i < NUM_SAMPLES; i++) {
    ASSERT_LE(re::length(inv.applyToPoint(proxy.randomPoint())), s.radius()) <<
      "should be contained within the shape";
  }
}

TEST(ProxyShape, containsPoint_test) {
  re::Sphere s(1.0);
  re::Transform m;
  m.scale(5, 3, 0.8);
  m.rotate(re::randf(0.0, 33.3*RE_PI), re::vec3::rand());

  const reProxyShape proxy(&s, m);
  for (unsigned int i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = proxy.randomPoint();
    ASSERT_TRUE(proxy.containsPoint(pt)) <<
      "should be true for all generated points";

    const re::vec3 ptOutside = m.applyToDir(re::normalize(s.randomPoint()) * re::randf(s.radius() * 1.1, s.radius() * 100.0));
    ASSERT_FALSE(proxy.containsPoint(ptOutside)) <<
      "should be false for all points outside the shape";
  }
}

#include "react/debug.h"

TEST(ProxyShape, intersects_test) {
  re::Sphere s(5.0);
  re::Transform m;
  m.scale(1, 1, 1);
  m.rotate(re::randf(0.0, 50.0*RE_PI), re::vec3::rand());

  const re::Transform inv = re::inverse(m);
  const reProxyShape proxy(&s, m);
  re::RayQuery result;
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = proxy.randomPoint();
    const re::vec3 rpt = s.randomPoint() + re::vec3(RE_FP_TOLERANCE, 0.0, 0.0);
    const re::vec3 ptOutside = m.applyToDir(re::normalize(rpt)* re::randf(s.radius() * 1.1, s.radius() * 10.0));
    const re::Ray ray(ptOutside, pt - ptOutside);
   
    ASSERT_TRUE(re::intersects(proxy, IDEN_TRANS, ray, result)) <<
      "should return true";

    ASSERT_FLOAT_EQ(re::length(result.normal), 1.0) <<
      "should have the result normal normalized";

    ASSERT_LE(re::abs(re::length(inv.applyToPoint(result.point)) - s.radius()), RE_FP_TOLERANCE) <<
      "should return the resulting intersection at the shape surface";

    // const re::vec3 perp = re::normalize(m.applyToDir(re::cross(ptOutside, re::vec3::rand(100.0))));
    // const re::Ray ray2(ptOutside - perp * re::randf(1.0, 100.0), perp);
  
    // ASSERT_FALSE(re::intersects(proxy, IDEN_TRANS, ray2, result)) <<
      // "should return false" << i;
  }
}
