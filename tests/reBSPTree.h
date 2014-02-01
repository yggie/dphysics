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
  
  reSphere* sp = world.allocator().alloc_new<reSphere>(1.0);
  reRigidBody* r2 = world.allocator().alloc_new<reRigidBody>(sp);
  world.add(r2);
  ASSERT_EQ(world.entities().size(), 2) << "add() should accept new entities";
  
  world.clear();
  ASSERT_EQ(world.entities().size(), 0) << "clear() should remove all entities";
}

