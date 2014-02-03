#ifndef RE_TEST_FIXTURES_H
#define RE_TEST_FIXTURES_H

#include "helpers.h"

class reQueryable;

#include <vector>

struct TestFixtures : public ::testing::Test {
  TestFixtures();
  virtual ~TestFixtures();
protected:
  void generateFixtures(unsigned int n);
  void clearFixtures();
  void clearOnlyQFixtures();
  
  std::vector<reQueryable*> fixtures;
private:
  SimpleAllocator _internalAllocator;
};

#endif

