#include "helpers.h"
#include "react/reWorld.h"
#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

struct reSolidTest : public testing::Test {
protected:
  reWorld world;
};

TEST_F(reSolidTest, HasIdentityRotationOnInit) {
  reRigidBody& body = world.build().RigidBody(reSphere(1.0));
  
  re::mat3 r = body.rot();
  ASSERT_MAT_EQ(r, re::mat3(1.0));
}

TEST_F(reSolidTest, SetFacingMethod) {
  reRigidBody& body = world.build().RigidBody(reSphere(1.0)).facing(re::vec3(1.0, 0.0, 0.0), re::vec3(0.0, 0.0, 1.0));
  
  re::mat3 r = body.rot();
  ASSERT_VEC_EQ(r * re::vec3(0, 1, 0), re::vec3(1, 0, 0));
  ASSERT_VEC_EQ(r * re::vec3(0, 0, 1), re::vec3(0, 0, 1));
  
  re::vec3 yv = re::vec3(0, 1, 0);
  re::vec3 zv = re::vec3(0, 0, 1);
  for (reUInt i = 0; i < 50; i++) {
    re::vec3 rd = re::normalize(re::vec3::random());
    re::vec3 r2 = re::normalize(re::vec3::random());
    body.facing(rd, r2);
    
    ASSERT_VEC_EQ(body.rot() * yv, rd);
    ASSERT_GE(reAbs(re::dot(body.rot() * zv, r2)), 0.0);
  }
}

