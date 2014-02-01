#include "helpers.h"

#include "react/reWorld.h"
#include "react/Collision/reBSPTree.h"

struct reBSPTreeTest : public testing::Test {
  reBSPTreeTest() : world(), tree((reBSPTree&)world.broadPhase()) { }
protected:
  reWorld world;
  reBSPTree& tree;
};

TEST_F(reBSPTreeTest, Creation) {
  ASSERT_EQ(tree.entities().size(), 0) <<
    "should be initialized with no entities";
  
  ASSERT_FALSE(tree.hasChildren()) <<
    "should not have any children when there are no entities";
  
  ASSERT_EQ(tree.depth, 0) <<
    "root node should be at depth 0";
}

