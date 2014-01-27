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
  
  reMatrix r = body.rot();
  ASSERT_MAT_EQ(r, reMatrix(1.0));
}

TEST_F(reSolidTest, SetFacingMethod) {
  reRigidBody& body = world.newRigidBody(reSphere(1.0)).facing(reVector(1.0, 0.0, 0.0));
  
  reMatrix r = body.rot();
  rePrint(r);
  ASSERT_VEC_EQ(r * reVector(0, 1, 0), reVector(1, 0, 0));
  ASSERT_VEC_EQ(r * reVector(0, 0, 1), reVector(0, 0, 1));
  
  reVector yv = reVector(0, 1, 0);
  for (reUInt i = 0; i < 50; i++) {
    reVector rd = reVector::random();
    body.facing(rd);
    ASSERT_VEC_EQ(body.rot() * yv, rd);
  }
}

