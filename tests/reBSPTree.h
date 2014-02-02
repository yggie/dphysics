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
  ASSERT_EQ(tree.size(), 0) <<
    "should be initialized with no entities";
  
  ASSERT_FALSE(tree.hasChildren()) <<
    "should not have any children when there are no entities";
  
  ASSERT_TRUE(tree.isRoot()) <<
    "should be the root node";
  
  ASSERT_TRUE(tree.isLeaf()) <<
    "should initially be the leaf node";
  
  ASSERT_EQ(tree.depth, 0) <<
    "root node should be at depth 0";
}

TEST_F(reBSPTreeTest, AddContainRemoveActions) {
  reSphere s(1.0);
  reRigidBody body(&s);
  reQueryable q(body);
  
  ASSERT_TRUE(tree.add(body)) <<
    "should be able to add entities";
  
  ASSERT_EQ(tree.size(), 1) <<
    "should increment the entity count by one";
  
  ASSERT_TRUE(tree.contains(body)) <<
    "should return true for the entity just added";
  
  ASSERT_TRUE(tree.remove(body)) <<
    "should be able to remove the added entity";
  
  ASSERT_EQ(tree.size(), 0) <<
    "should have an empty entity list";
}

