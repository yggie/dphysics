#include "helpers.h"

#include "react/Collision/Shapes/shapes.h"
#include "react/Entities/Rigid.h"

TEST(Rigid, Constructor_test) {
  re::Sphere s(1.0);
  re::Rigid body(s);
  ASSERT_TRUE(re::similar(body.rot(), re::mat3(1.0))) <<
    "should have no initial rotation";
}

TEST(Rigid, SetFacingMethod) {
  re::Sphere s(1.0);
  re::Rigid body(s);

  body.facing(re::vec3(1.0, 0.0, 0.0), re::vec3(0.0, 0.0, 1.0));
  
  re::mat3 r = body.rot();
  ASSERT_TRUE(re::similar(r * re::vec3(0, 1, 0), re::vec3(1, 0, 0))) <<
    "should rotate the y-axis to the facing direction";
  ASSERT_TRUE(re::similar(r * re::vec3(0, 0, 1), re::vec3(0, 0, 1))) <<
    "should rotate the z-axis to the up direction";
  
  re::vec3 yv = re::vec3(0, 1, 0);
  re::vec3 zv = re::vec3(0, 0, 1);
  for (reUInt i = 0; i < 50; i++) {
    re::vec3 rd = re::vec3::unit();
    re::vec3 r2 = re::vec3::unit();
    body.facing(rd, r2);
    
    ASSERT_TRUE(re::similar(body.rot() * yv, rd)) <<
      "should rotate the y-axis to the facing direction";
    ASSERT_GE(re::abs(re::dot(body.rot() * zv, r2)), 0.0) <<
      "should have the new z-axis facing in the same direction as the input";
  }
}

