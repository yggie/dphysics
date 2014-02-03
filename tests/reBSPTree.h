#include "TestFixtures.h"

#include "react/Collision/reBSPTree.h"
#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

struct reBSPTreeTest : public ::testing::Test {
  reBSPTreeTest() : tree(SHARED_ALLOCATOR), fixtures() { }
protected:
  void generateFixtures(unsigned int n);
  
  reBSPTree tree;
  std::vector<reRigidBody*> fixtures;
};

void reBSPTreeTest::generateFixtures(unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    reSphere* s = SHARED_ALLOCATOR.alloc_new<reSphere>(1.0);
    reRigidBody* body = SHARED_ALLOCATOR.alloc_new<reRigidBody>(s);
    body->setPos(re::vec3::rand(100.0));
    fixtures.push_back(body);
  }
}

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
  generateFixtures(1000);
  reRigidBody& body = *fixtures.at(0);
  
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
  
  for (reRigidBody* body : fixtures) {
    ASSERT_TRUE(tree.add(*body)) <<
      "should be able to add unique entities to the structure";
    
    ASSERT_FALSE(tree.add(*body)) <<
      "should NOT be able to add repeated entities to the structure";
  }
  
  ASSERT_EQ(tree.size(), fixtures.size()) <<
    "should have size equal to the number added";
  
  for (reRigidBody* body : fixtures) {
    ASSERT_TRUE(tree.remove(*body)) <<
      "should be able to remove contained entities";
  }
  ASSERT_EQ(tree.size(), 0) <<
    "should be empty after removing all entities";
  
  for (reRigidBody* body : fixtures) {
    ASSERT_TRUE(tree.add(*body)) <<
      "should be able to add unique entities to the structure";
  }
  tree.clear();
  ASSERT_EQ(tree.size(), 0) <<
    "should be able to clear all entities";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reBSPTreeTest, TreeBalancing) {
  generateFixtures(1000);
  
  for (reRigidBody* body : fixtures) {
    ASSERT_TRUE(tree.add(*body)) <<
      "should be able to add unique entities to the structure";
  }
  
  tree.rebalance();
  
  ASSERT_EQ(tree.size(), fixtures.size()) <<
    "should not change in size when rebalanced";
  
  for (reEnt& ent : tree.entities()) {
    ent.setPos(re::vec3::rand(250.0));
  }
  tree.rebalance();
  
  ASSERT_EQ(tree.size(), fixtures.size()) <<
    "should not change in size when rebalanced";
  
  tree.clear();
  ASSERT_EQ(tree.size(), 0) <<
    "should be able to remove all entities in the structure";
  
  ASSERT_NO_MEM_LEAKS();
}

