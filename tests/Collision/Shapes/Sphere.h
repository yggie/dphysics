#include "helpers.h"

#include "react/Collision/Shapes/shapes.h"

TEST(Sphere, ConstructorAndProperties_test) {
  re::Sphere s(3.0);
  
  ASSERT_FLOAT_EQ(s.radius(), 3.0) << "should create a sphere with given radius";
  
  ASSERT_TRUE(s.type() == reShape::SPHERE) << "should have the correct type";

  ASSERT_FLOAT_EQ(s.shell(), 3.0) << "shell thickness and radius should be equivalent";
  
  ASSERT_EQ(s.numVerts(), 1) << "should only have one vertex";
  
  ASSERT_TRUE(re::similar(s.vert(0), re::vec3(0.0, 0.0, 0.0))) <<
    "should have its only vertex at the origin";

  ASSERT_LE(re::abs(s.volume() - RE_PI*4.0*9.0), RE_FP_TOLERANCE * s.volume()) <<
    "should return the volume within tolerance";

  re::Sphere s2(s);
  ASSERT_FLOAT_EQ(s2.radius(), s.radius()) <<
    "should copy the radius";
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

TEST(Sphere, randomPoint_test) {
  re::Sphere s(3.0);

  for (unsigned int i = 0; i < NUM_SAMPLES; i++) {
    ASSERT_LE(re::lengthSq(s.randomPoint()), 9.0) <<
      "should generate random points inside the sphere";
  }
}

TEST(Sphere, containsPoint_test) {
  re::Sphere s(5.0);

  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = s.randomPoint();
    
    ASSERT_TRUE(s.containsPoint(pt)) <<
      "should be true for any generated random point";
    
    const re::vec3 ptOutside = re::normalize(pt) * re::randf(5.1, 100.0);
    ASSERT_FALSE(s.containsPoint(ptOutside)) <<
      "should be false for any generated points outside the sphere";
  }
}

TEST(Sphere, intersects_test) {
  re::Sphere s(2.0);
  
  re::RayQuery result;
  for (reUInt i = 0; i < NUM_SAMPLES; i++) {
    const re::vec3 pt = s.randomPoint();
    const re::vec3 ptOutside = re::normalize(s.randomPoint()) * re::randf(s.radius() * 1.1, 10.0 * s.radius());
    const re::Ray ray(ptOutside, pt - ptOutside);

    ASSERT_TRUE(re::intersects(s, IDEN_TRANS, ray, result)) <<
      "should return true";

    ASSERT_FLOAT_EQ(re::length(result.normal), 1.0) <<
      "should have the normal vector normalized";

    ASSERT_LE(re::abs(re::length(result.point) - s.radius()), 2*RE_FP_TOLERANCE) <<
      "should have the intersection point at the sphere surface";

    ASSERT_LE(re::length(ptOutside) - result.depth, s.radius() + RE_FP_TOLERANCE) <<
      "should have the depth within a valid range";
    
    const re::vec3 perp = re::normalize(re::cross(ptOutside, re::vec3::rand()));
    const re::Ray ray2(ptOutside - re::randf(1.0, 100.0)*perp, perp);

    ASSERT_FALSE(re::intersects(s, IDEN_TRANS, ray2, result)) <<
      "should return false";
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
