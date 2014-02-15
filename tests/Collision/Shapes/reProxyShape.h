#include "helpers.h"

#include "react/Collision/Shapes/shapes.h"

TEST(ProxyShapeTest, Creation) {
  re::Sphere s(0.1);
  reProxyShape ps(&s);
  
  ASSERT_TRUE(ps.type() == reShape::PROXY) << "should have the correct type";
  
  ASSERT_TRUE(ps.shape() == &s) << "should have the correct base shape";
}

TEST(ProxyShapeTest, QueryTest) {
  re::Sphere s(5.0);
  reTransform m;
  m.scale(1, 2, 3);
  m.rotate(re::randf(0.0, 50.0*RE_PI), re::vec3::rand());
  reProxyShape ps(&s, m);
  reRayQueryResult res;
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = ps.randomPoint();
    ASSERT_TRUE(ps.containsPoint(pt)) <<
      "should be true for any generated point";
    
    re::vec3 ptOutside = re::vec3::rand() * 1000.0;
    while (ps.containsPoint(ptOutside)) {
      ptOutside = re::vec3::rand() * 1000.0;
    }
    ASSERT_FALSE(ps.containsPoint(ptOutside)) <<
      "should be false for any generated points outside the sphere";
    
    reRayQuery query;
    query.origin = ptOutside;
    query.dir = (pt - ptOutside);
    EXPECT_TRUE(ps.intersectsRay(query, res)) <<
      "should intersect segments built from a point outside to a point inside the sphere " << pt[0] << "," << pt[1] << "," << pt[2];
  }
}
