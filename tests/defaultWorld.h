#include "helpers.h"

#include "react/reWorld.h"
#include "react/Collision/Shapes/Sphere.h"
#include "react/Entities/Rigid.h"
#include "react/Collision/reBroadPhase.h"

/**
 * Integration test using default world parameters
 */

struct DefaultWorldTest : public testing::Test {
  protected:
    reWorld world;
};

TEST_F(DefaultWorldTest, AddRemoveClearActions) {
  re::Rigid& r = world.build().Rigid(re::Sphere(1.0));
  ASSERT_EQ(world.entities().size(), 1) << "can create new entities";
  
  world.add(r);
  ASSERT_EQ(world.entities().size(), 1) << "add() should reject repeated entities";
  
  re::Sphere* sp = world.allocator().alloc_new<re::Sphere>(1.0);
  re::Rigid* r2 = world.allocator().alloc_new<re::Rigid>(*sp);
  world.add(*r2);
  ASSERT_EQ(world.entities().size(), 2) << "add() should accept new entities";
  
  world.destroy(*r2);
  ASSERT_EQ(world.entities().size(), 1) << "remove() should remove the specified entity";
  
  world.clear();
  ASSERT_EQ(world.entities().size(), 0) << "clear() should remove all entities";
}

