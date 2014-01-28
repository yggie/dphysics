#include "helpers.h"
#include "react/reWorld.h"
#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

struct reSolidTest : public testing::Test {
protected:
  reWorld world;
};

TEST_F(reSolidTest, HasIdentityRotationOnInit) {
  reRigidBody& body = world.newRigidBody(reSphere(1.0));
  
  re::mat3 r = body.rot();
  ASSERT_MAT_EQ(r, re::mat3(1.0));
}

TEST_F(reSolidTest, SetFacingMethod) {
  reRigidBody& body = world.newRigidBody(reSphere(1.0)).facing(re::vec(1.0, 0.0, 0.0), 1.5);
  
  re::mat3 r = body.rot();
  ASSERT_VEC_EQ(r * re::vec(0, 1, 0), re::vec(1, 0, 0));
  ASSERT_VEC_EQ(r * re::vec(0, 0, 1), re::vec(0, 0, 1));
  
  re::vec yv = re::vec(0, 1, 0);
  for (reUInt i = 0; i < 50; i++) {
    re::vec rd = re::vec::random();
    body.facing(rd);
    ASSERT_VEC_EQ(body.rot() * yv, rd);
  }
}

