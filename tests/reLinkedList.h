#include "helpers.h"

#include "react/Collision/Shapes/reSphere.h"
#include "react/Utilities/reLinkedList.h"

namespace {
  reSphere s(1.0);
  
  template <class T>
  T copyOf(T v) {
    return T(v);
  }
}

struct reLinkedListTest : public ::testing::Test {
  reLinkedListTest() : list(SHARED_ALLOCATOR), fixtures() { }
  ~reLinkedListTest();
protected:
  void generateFixtures(unsigned int n);
  
  reLinkedList<int*> list;
  std::vector<int*> fixtures;
};

reLinkedListTest::~reLinkedListTest() {
  for (int* ptr : fixtures) {
    delete ptr;
  }
  fixtures.clear();
}

void reLinkedListTest::generateFixtures(unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    fixtures.push_back(new int);
  }
}

TEST_F(reLinkedListTest, Creation) {
  ASSERT_EQ(list.size(), 0) <<
    "should have an initial size of zero";
  
  for (int* ptr : list) {
    ASSERT_TRUE(false) <<
      "should not be iteratable when empty";
    printf("%x\n", *ptr);
  }
  
  list.clear();
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reLinkedListTest, AddRemoveClearActions) {
  generateFixtures(1000);
  
  list.add(fixtures.at(0));
  ASSERT_EQ(list.size(), 1) <<
    "should have increased in size when adding an element";
  
  list.add(fixtures.at(1));
  ASSERT_EQ(list.size(), 2) <<
    "should have increased in size when adding an additional element";
  
  list.remove(fixtures.at(0));
  ASSERT_EQ(list.size(), 1) <<
    "should have decreased in size when removing an element contained";
  
  for (int* ptr : list) {
    ASSERT_TRUE(ptr == fixtures.at(1)) <<
      "should be able to iterate over the remaining contents";
  }
  
  list.clear();
  ASSERT_EQ(list.size(), 0) <<
    "should remove all elements in the list";
  
  for (int* ptr : fixtures) {
    list.add(ptr);
  }
  ASSERT_EQ(list.size(), fixtures.size()) <<
    "should have a list size reflecting all current elements";
  
  for (int* ptr : fixtures) {
    ASSERT_TRUE(list.remove(ptr)) <<
      "should be able to manually remove contained elements";
  }
  
  ASSERT_EQ(list.size(), 0) <<
    "should have no elements remaining";
  
  list.clear();
  ASSERT_EQ(list.size(), 0) <<
    "should do nothing";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reLinkedListTest, Appending) {
  generateFixtures(1000);
  
  for (int* ptr : fixtures) {
    list.add(ptr);
  }
  
  ASSERT_EQ(list.size(), fixtures.size()) <<
    "should have the correct size";
  
  reLinkedList<int*> list2(SHARED_ALLOCATOR);
  
  list2.append(list);
  ASSERT_EQ(list.size(), list2.size()) <<
    "should have equal sizes";
  
  list.append(list2);
  ASSERT_EQ(list.size(), 2*fixtures.size()) <<
    "should have new size equal to sum of appending list sizes";
  
  list.clear();
  list2.clear();
  ASSERT_EQ(list.size(), 0) <<
    "should be empty after clearing";
  ASSERT_EQ(list2.size(), 0) <<
    "should be empty after clearing";
  
  ASSERT_NO_MEM_LEAKS();
}

TEST_F(reLinkedListTest, Copying) {
  generateFixtures(1000);
  
  for (int* ptr : fixtures) {
    list.add(ptr);
  }
  
  auto list2(list);
  ASSERT_EQ(list2.size(), list.size()) <<
    "should have equal sizes after copying";
  
  for (int* ptr : list) {
    ASSERT_TRUE(list2.contains(ptr)) <<
      "should have the elements copied correctly";
  }
  
  reLinkedList<int*> list3 = copyOf(copyOf(list));
  
  ASSERT_EQ(list3.size(), list.size()) <<
    "should work with nested constructors";
  
  list.clear();
  list2.clear();
  list3.clear();
  ASSERT_EQ(list.size(), 0) <<
    "should be empty after clearing";
  ASSERT_EQ(list2.size(), 0) <<
    "should be empty after clearing";
  ASSERT_EQ(list3.size(), 0) <<
    "should be empty after clearing";
  
  ASSERT_NO_MEM_LEAKS();
}

