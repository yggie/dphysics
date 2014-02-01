#include "helpers.h"

#include "react/reWorld.h"
#include "react/Collision/reBroadPhase.h"

/**
 * Integration test using default world parameters
 */

struct DefaultWorldTest : public testing::Test {
  protected:
    reWorld world;
};

TEST_F(DefaultWorldTest, AddRemoveClearActions) {
  reRigidBody& r = world.build().RigidBody(reSphere(1.0));
  ASSERT_EQ(world.entities().size(), 1) << "can create new entities";
  
  world.add(r);
  ASSERT_EQ(world.entities().size(), 1) << "add() should reject repeated entities";
  
  reSphere* sp = world.allocator().alloc_new<reSphere>(1.0);
  reRigidBody* r2 = world.allocator().alloc_new<reRigidBody>(sp);
  world.add(*r2);
  ASSERT_EQ(world.entities().size(), 2) << "add() should accept new entities";
  
  world.destroy(*r2);
  ASSERT_EQ(world.entities().size(), 1) << "remove() should remove the specified entity";
  
  world.clear();
  ASSERT_EQ(world.entities().size(), 0) << "clear() should remove all entities";
}
