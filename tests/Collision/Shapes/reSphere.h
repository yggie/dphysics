#include "helpers.h"

#include "react/Collision/Shapes/reSphere.h"

TEST(SphereTest, Creation) {
  reSphere s(3.0);
  
  ASSERT_FLOAT_EQ(s.radius(), 3.0) << "should create a sphere with given radius";
  
  ASSERT_TRUE(s.type() == reShape::SPHERE) << "should have the correct type";
}

TEST(SphereTest, ChangingRadius) {
  reSphere s(1.0);
  
  ASSERT_FLOAT_EQ(s.withRadius(50.0).radius(), 50.0) << "can change the radius with a chainable method";
  
  s.setRadius(15.0);
  ASSERT_FLOAT_EQ(s.radius(), 15.0) << "can change the radius with a setter";
}

TEST(SphereTest, VertexData) {
  reSphere s(33.3);
  
  ASSERT_FLOAT_EQ(s.shell(), 33.3) << "shell thickness and radius should be equivalent";
  
  ASSERT_EQ(s.numVerts(), 1) << "should only have one vertex";
  
  ASSERT_TRUE(re::similar(s.vert(0), re::vec3(0.0, 0.0, 0.0))) <<
    "should have its only vertex at the origin";
}

TEST(SphereTest, QueryTest) {
  reSphere s(5.0);
  reRayQueryResult res;
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = s.randomPoint();
    ASSERT_LE(re::lengthSq(pt), 25.0) <<
      "should generate random points inside the sphere";
    
    ASSERT_TRUE(s.containsPoint(pt)) <<
      "should be true for any generated point";
    
    const re::vec3 ptOutside = re::normalize(pt) * re::randf(5.1, 100.0);
    ASSERT_FALSE(s.containsPoint(ptOutside)) <<
      "should be false for any generated points outside the sphere";
    
    reRayQuery query;
    query.origin = ptOutside;
    query.dir = (pt - ptOutside);
    ASSERT_TRUE(s.intersectsRay(query, res)) <<
      "should intersect segments built from a point outside to a point inside the sphere";
  }
}

TEST(SphereTest, RayIntersectionSadPaths) {
  reSphere s(2.0);
  
  reRayQueryResult res;
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 ptOutside = re::vec3::unit() * re::randf(2.1, 1111.1);
    
    reRayQuery query;
    const re::vec3 edge = re::normalize(re::cross(ptOutside, re::vec3::rand()));
    query.origin = ptOutside - edge * re::randf(1e2, 1e3);
    query.dir = edge;
    
    ASSERT_FALSE(s.intersectsRay(query, res)) <<
      "should not intersect with rays outside the sphere";
  }
}

TEST(SphereTest, FastPlaneIntersection) {
  reSphere s(1.0);
  
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 origin = re::vec3::rand(1000.0);
    const re::vec3 normal = re::normalize(re::vec3::rand());
    
    const reFloat d = re::dot(origin, normal);
    re::PlaneQuery::FastResult r = s.fastPlaneIntersect(normal, origin);
    
    if (re::abs(d) < 1.0 + RE_FP_TOLERANCE) {
      ASSERT_TRUE(r == re::PlaneQuery::INTERSECTS) <<
        "should intersect when the dot product is less than the radius loop@" << i;
    } else if (d > 0.0) {
      ASSERT_TRUE(r == re::PlaneQuery::BEHIND) <<
        "should be behind the plane loop@" << i;
    } else {
      ASSERT_TRUE(r == re::PlaneQuery::FRONT) <<
        "should be in front of the plane loop@" << i;
    }
  }
}

