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
  
  ASSERT_EQ(tree.depth(), 0) <<
    "root node should be at depth 0";
}

TEST_F(reBSPTreeTest, AddContainRemoveActions) {
  generateFixtures(1000);
  reRigidBody& body = *fixtures.at(0);
  
  ASSERT_FALSE(tree.contains(body)) <<
    "should return false for an entity which has not been added";
  
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

TEST_F(reBSPTreeTest, Sampling) {
  generateFixtures(1000);
  
  for (reRigidBody* body : fixtures) {
    ASSERT_TRUE(tree.add(*body)) <<
      "should be able to add unique entities";
  }
  
  auto list = tree.sample(300);
  
  for (reEnt* body : list) {
    ASSERT_TRUE(tree.contains(*body)) <<
      "should contain the body from the sample";
  }
  
  tree.clear();
  list.clear();
  
  ASSERT_NO_MEM_LEAKS();
}

namespace {
  unsigned int placements;
  unsigned int maxPlacements;
  unsigned int minPlacements;
  void prep(unsigned int total) {
    placements = 0;
    maxPlacements = 0;
    minPlacements = total;
  }
  bool countPlacements(reBSPNode& node) {
    placements += node.placements();
    if (node.placements() < minPlacements) {
      minPlacements = node.placements();
    }
    if (node.placements() > maxPlacements) {
      maxPlacements = node.placements();
    }
    return false;
  }
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
  
  ASSERT_FALSE(tree.isLeaf()) <<
    "should branch off when containing a large number of entities";
  
  ASSERT_LT(tree.placements(), fixtures.size()/3) <<
    "should have dispersed the elements throughout the tree";
  
  prep(fixtures.size());
  tree.execute(countPlacements);
  ASSERT_EQ(placements, fixtures.size()) <<
    "should not lose references in the structure";
  
  ASSERT_LE(maxPlacements, fixtures.size()/10) <<
    "should have a reasonably balanced tree";

  reBPMeasure m = tree.measure();
  ASSERT_EQ(m.references, m.entities) <<
    "should contain an equal number of references and entities";
  
  for (reEnt* ent : tree.entities()) {
    ent->setPos(re::vec3::rand(250.0));
  }
  tree.rebalance();

  ASSERT_FALSE(tree.isLeaf()) <<
    "should not lose all its nodes after rebalancing";

  prep(fixtures.size());
  tree.execute(countPlacements);
  ASSERT_LE(maxPlacements, fixtures.size()/10) <<
    "should still have a reasonably balanced tree";
  
  m = tree.measure();
  ASSERT_EQ(m.references, m.entities) <<
    "should keep a 1:1 ratio between references and entities";
  
  ASSERT_EQ(tree.size(), fixtures.size()) <<
    "should not change in size when rebalanced";
  
  tree.clear();
  ASSERT_EQ(tree.size(), 0) <<
    "should be able to remove all entities in the structure";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reBSPTreeTest, RayQueries) {
  const int N = 20; // too high will make it slow
  generateFixtures(N*N);
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      reRigidBody* body = fixtures.at(N*i + j);
      body->setPos(3.0 * re::vec3(i - N/2, j - N/2, 0.0));
      ASSERT_TRUE(tree.add(*body)) <<
        "should be able to add new entities";
    }
  }
  
  reRayQuery query;
  query.origin = re::vec3(0.0, 0.0, 100.0);
  unsigned int II = 0;
  for (reRigidBody* body : fixtures) {
    query.dir = re::normalize(body->center() - query.origin);
//    re::SingleResult res = tree.query().withRay(start, direction);
    re::RayResult res = tree.queryWithRay(query);
    ASSERT_TRUE(body == res.entity) <<
      "should return the correct entity";
  }
  
  tree.rebalance();
  ASSERT_EQ(tree.size(), fixtures.size()) <<
    "should not lose elements when rebalancing";
  
  placements = 0;
  tree.execute(countPlacements);
  ASSERT_EQ(placements, fixtures.size()) <<
    "should not lose references in the structure";
  
  query.origin = re::vec3(0.0, 0.0, 1000.0);
  II = 0;
  for (reRigidBody* body : fixtures) {
    query.dir = re::normalize(body->center() - query.origin);
    re::RayResult res = tree.queryWithRay(query);
    ASSERT_TRUE(body == res.entity) <<
      "should still work after rebalancing loop@" << II;
    II++;
  }
}

