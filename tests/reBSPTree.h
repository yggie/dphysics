#include "helpers.h"

#include "react/reWorld.h"
#include "react/Collision/reBroadPhase.h"

struct BSPTreeTest : public testing::Test {
  protected:
    reWorld world;
};

TEST_F(BSPTreeTest, AddClearActions) {
  
  reRigidBody& r = world.newRigidBody(reSphere(1.0));
  ASSERT_EQ(world.entities().size(), 1) << "can create new entities";
  
  world.add(&r);
  ASSERT_EQ(world.entities().size(), 1) << "add() should reject repeated entities";
  
  reRigidBody* r2 = new reRigidBody(nullptr);
  world.add(r2);
  ASSERT_EQ(world.entities().size(), 2) << "add() should accept new entities";
  
  world.clear();
  ASSERT_EQ(world.entities().size(), 0) << "clear() should remove all entities";
}

