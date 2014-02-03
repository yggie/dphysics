#include "TestFixtures.h"
#include "react/Utilities/reEntList.h"
#include "react/Utilities/reLinkedList.h"
#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"

struct reEntListTest : public TestFixtures {
  reEntListTest() : list(SHARED_ALLOCATOR) { }
protected:
  bool ensureSorted(reEntList& list);
  
  reEntList list;
};

bool reEntListTest::ensureSorted(reEntList& list) {
  unsigned int ID = 0;
  for (reEnt& ent : list) {
    if (ent.id() < ID) return false;
    ID = ent.id();
  }
  return true;
}

TEST_F(reEntListTest, Creation) {
  
  ASSERT_EQ(list.size(), 0) <<
    "should have an initial size of zero";
  
  for (reEnt& ent : list) {
    ASSERT_TRUE(false) <<
      "should not be iteratable when empty";
    printf("%d\n", ent.id());
  }
  
  ASSERT_TRUE(list.first() == nullptr) <<
    "should not have any nodes";
  
  ASSERT_TRUE(list.last() == nullptr) <<
    "should not have any nodes";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reEntListTest, AddRemoveClearActions) {
  generateFixtures(1000);
  
  ASSERT_EQ(list.size(), 0) <<
    "should have an initial size of zero";
    
  list.add(*fixtures.at(0));
  ASSERT_EQ(list.size(), 1) <<
    "should increase in size when adding an element";
  
  ASSERT_FALSE(list.add(*fixtures.at(0))) <<
    "should be unable to add the same element";
  ASSERT_EQ(list.size(), 1) <<
    "should not increase in size when failed to add an element";
  
  ASSERT_TRUE(list.add(*fixtures.at(1))) <<
    "should be able to add a new element";
  ASSERT_EQ(list.size(), 2) <<
    "should increase in size when successfully adding an element";
  
  ASSERT_TRUE(list.remove(*fixtures.at(0))) <<
    "should be able to remove an item which has been added";
  ASSERT_EQ(list.size(), 1) <<
    "should decrease in size when removing an element contained in the list";
  
  for (reEnt& ent : list) {
    ASSERT_EQ(ent.id(), fixtures.at(1)->ent.id()) <<
      "should be able to iterate over the remaining elements";
  }
  
  list.clear();
  ASSERT_EQ(list.size(), 0) <<
    "should be able to clear all elements";
  
  for (reQueryable* q : fixtures) {
    ASSERT_TRUE(list.add(*q)) <<
      "should be able to add new elements";
  }
  
  ASSERT_EQ(list.size(), fixtures.size()) <<
    "should be able to repeatedly add elements";
  
  unsigned int II = 0;
  for (reEnt& ent : list) {
    (void)ent.id(); // just to stop the compiler from complaining
    II++;
  }
  ASSERT_EQ(II, fixtures.size()) <<
    "should be able to iterate over all the contained elements";
  
  for (reQueryable* q : fixtures) {
    ASSERT_TRUE(list.remove(*q)) <<
      "should be able to remove contained elements";
  }
  
  ASSERT_EQ(list.size(), 0) <<
    "should be able to manually remove all elements";
  ASSERT_TRUE(list.first() == nullptr && list.last() == nullptr) <<
    "should have no elements remaining";
  
  ASSERT_EQ(list.size(), 0) <<
    "clearing an empty list should do nothing";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reEntListTest, SortingAndSampling) {
  for (unsigned int i = 0; i < NUM_REPEATS; i++) {
    generateFixtures(1000);
    
    for (reQueryable* q : fixtures) {
      ASSERT_TRUE(list.add(*q)) <<
        "should be able to add new elements";
    }
    
    unsigned int ID = 0;
    for (reEnt& ent : list) {
      ASSERT_GT(ent.id(), ID) <<
        "should be ordered according to the allocated entity id";
      ID = ent.id();
    }
    
    reLinkedList<reEnt*> s = list.sample(300);
    
    ASSERT_EQ(s.size(), 300) <<
      "should return a sample of the required size";
    
    ASSERT_TRUE(ensureSorted(list)) <<
      "should be ordered according to the allocated entity id";
    
    list.clear();
    clearFixtures();
  }
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reEntListTest, ExchangingElements) {
  generateFixtures(10);
  reEntList list2(SHARED_ALLOCATOR);
  
  for (reQueryable* q : fixtures) {
    ASSERT_TRUE(list.add(*q)) <<
      "should be able to add new elements";
  }
  
  for (reQueryable* q : fixtures) {
    ASSERT_TRUE(list.remove(*q)) <<
      "should be able to remove contained elements";
    
    ASSERT_TRUE(list2.add(*q)) <<
      "should be able to add new elements";
  }
  
  unsigned int II = 0;
  for (reEnt& ent : list2) {
    ASSERT_EQ(ent.id(), fixtures.at(II++)->ent.id()) <<
      "should be placed in the correct order";
  }
  list2.clear();
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reEntListTest, AppendingAndCopying) {
  generateFixtures(500);
  
  reEntList list2(SHARED_ALLOCATOR);
  ASSERT_EQ(list2.size(), 0) <<
    "should be initialized as an empty list";
  
  list.append(list2);
  list2.append(list);
  
  ASSERT_EQ(list.size(), 0) <<
    "should not be affected when appending empty lists";

  ASSERT_EQ(list2.size(), 0) <<
    "should not be affected when appending empty lists";

  for (reQueryable* q : fixtures) {
    ASSERT_TRUE(list.add(*q)) <<
      "should be able to add new elements";
  }
  list2.append(list);
  
  ASSERT_EQ(list2.size(), list.size()) <<
    "should be equal in size if appending to an empty list";
  ASSERT_FALSE(list2.at(list2.size() - 1) == nullptr) <<
    "size should accurately reflect the actual stored size";
  
  reEntList list3(list);
  ASSERT_EQ(list3.size(), list.size()) <<
    "should be equal in size if copying from another list";
  
  for (unsigned int i = 0; i < fixtures.size(); i++) {
    ASSERT_EQ(list.at(i)->id(), list2.at(i)->id()) <<
      "should have elements in the correct order";
    ASSERT_EQ(list.at(i)->id(), list3.at(i)->id()) <<
      "should have elements in the correct order";
  }
  
  list3.append(list);
  ASSERT_EQ(list3.size(), list.size()) <<
    "should not be able to append when elements are equal";
  ASSERT_FALSE(list3.at(list3.size() - 1) == nullptr) <<
    "size should accurately reflect the actual stored size";
  
  for (unsigned int i = 0; i < NUM_REPEATS; i++) {
    list2.append(list);
  }
  ASSERT_EQ(list2.size(), list.size()) <<
    "repeated appending should do nothing";
  
  list.clear();
  list2.clear();
  list3.clear();
  
  unsigned int II = 0;
  for (reQueryable* q : fixtures) {
    switch (II++ % 3) {
      case 0:
        list.add(*q);
        break;
      case 1:
        list2.add(*q);
        break;
      case 2:
        list3.add(*q);
        break;
    }
  }
  
  ASSERT_TRUE(ensureSorted(list) && ensureSorted(list2) && ensureSorted(list3)) << "should be sorted";
  
  unsigned int sz1 = list.size();
  unsigned int sz2 = list2.size();
  unsigned int sz3 = list3.size();
  
  list3.append(list);
  ASSERT_TRUE(ensureSorted(list3)) <<
    "should be ordered according to the allocated entity id";
  ASSERT_EQ(list3.size(), sz1 + sz3) <<
    "should be able to append whole lists with unique elements";
  ASSERT_FALSE(list3.at(list3.size() - 1) == nullptr) <<
    "size should accurately reflect the actual stored size";
  
  list.append(list2);
  ASSERT_TRUE(ensureSorted(list)) <<
    "should be ordered according to the allocated entity id";
  ASSERT_EQ(list.size(), sz1 + sz2) <<
    "should be able to append whole lists with unique elements";
  ASSERT_FALSE(list.at(list.size() - 1) == nullptr) <<
    "size should accurately reflect the actual stored size";
  
  list2.append(list3);
  ASSERT_EQ(list2.size(), fixtures.size()) <<
    "should be able to append whole lists with unique elements";
  ASSERT_TRUE(ensureSorted(list2)) <<
    "should be ordered according to the allocated entity id";
  
  list.append(list3);
  ASSERT_TRUE(ensureSorted(list)) <<
    "should be ordered according to the allocated entity id";
  ASSERT_EQ(list.size(), fixtures.size()) <<
    "should be able to append the unique elements of a similar list";
  
  list.clear();
  list2.clear();
  list3.clear();
  
  ASSERT_NO_MEM_LEAKS();
}


