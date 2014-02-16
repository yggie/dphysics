#include "helpers.h"

#include "react/Collision/Shapes/Sphere.h"

TEST(Sphere, Constructor_test) {
  re::Sphere s(3.0);
  
  ASSERT_FLOAT_EQ(s.radius(), 3.0) << "should create a sphere with given radius";
  
  ASSERT_TRUE(s.type() == reShape::SPHERE) << "should have the correct type";
}

TEST(Sphere, withRadius_test) {
  re::Sphere s(1.0);
  
  ASSERT_FLOAT_EQ(s.withRadius(50.0).radius(), 50.0) << "can change the radius with a chainable method";
}

TEST(Sphere, setRadius_test) {
  re::Sphere s(1.0);

  s.setRadius(15.0);
  ASSERT_FLOAT_EQ(s.radius(), 15.0) << "can change the radius with a setter";
}

TEST(Sphere, volume_test) {
  for (unsigned int i = 0; i < NUM_REPEATS; i++) {
    const float r = re::randf(1.0, 100.0);
    const re::Sphere s(r);
    ASSERT_LE(re::abs(s.volume() - RE_PI*4.0*r*r*r/3.0), 0.001*s.volume()) <<
      "should return the volume within tolerance";
  }
}

TEST(Sphere, VertexData_test) {
  re::Sphere s(33.3);
  
  ASSERT_FLOAT_EQ(s.shell(), 33.3) << "shell thickness and radius should be equivalent";
  
  ASSERT_EQ(s.numVerts(), 1) << "should only have one vertex";
  
  ASSERT_TRUE(re::similar(s.vert(0), re::vec3(0.0, 0.0, 0.0))) <<
    "should have its only vertex at the origin";
}

/**
TEST(Sphere, Something_test) {
  re::Sphere s(5.0);
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

TEST(Sphere, SomeOtherThing_test) {
  re::Sphere s(2.0);
  
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

/**
TEST(Sphere, locationInPlane_test) {
  re::Sphere s(1.0);
  
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 origin = re::vec3::rand(5.0);
    const re::vec3 normal = re::normalize(re::vec3::rand());
    
    const reFloat d = re::dot(origin, normal);
    re::Plane::Location loc = s.locationInPlane(re::Plane(normal, origin));
    
    if (re::abs(d) < 1.0 + RE_FP_TOLERANCE) {
      ASSERT_TRUE(loc == re::Plane::ON_PLANE || (re::abs(re::abs(d) - 1.0) < RE_FP_TOLERANCE)) <<
        "should detect when the object intersects the plane within a tolerance";
    } else if (d > 0.0) {
      ASSERT_TRUE(loc == re::Plane::BACK_OF_PLANE) <<
        "should detect when the object is at the back of the plane";
    } else {
      ASSERT_TRUE(loc == re::Plane::FRONT_OF_PLANE) <<
        "should detect when the object is at the front of the plane";
    }
  }
}
*/
