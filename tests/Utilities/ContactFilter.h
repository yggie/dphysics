#include "helpers.h"

#include "react/Utilities/ContactFilter.h"

#include "react/Collision/Shapes/Sphere.h"
#include "react/Entities/reRigidBody.h"
#include "react/Entities/StaticBody.h"

TEST(ContactFilter, filter_test) {
  re::Sphere s(1.0);
  reRigidBody a(&s);
  reRigidBody b(&s);
  re::StaticBody c(&s);
  re::StaticBody d(&s);

  re::ContactFilter cf;

  ASSERT_TRUE(cf.filter(a, b)) <<
    "should return true for collisions between two rigid bodies";

  ASSERT_TRUE(cf.filter(a, c)) <<
    "should return true for collisions between a rigid and a static body";

  ASSERT_TRUE(cf.filter(d, b)) <<
    "should return true for collisions between a rigid and a static body";

  ASSERT_FALSE(cf.filter(c, d)) <<
    "should return false for collisions between two static bodies";
}
