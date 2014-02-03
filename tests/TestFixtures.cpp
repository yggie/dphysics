#include "TestFixtures.h"

#include "react/Utilities/reEntList.h"
#include "react/Collision/Shapes/shapes.h"
#include "react/Entities/reRigidBody.h"

namespace {
  reSphere s(1.0);
}

TestFixtures::TestFixtures() : fixtures(), _internalAllocator() {
  // do nothing
}

TestFixtures::~TestFixtures() {
  clearFixtures();
}

void TestFixtures::generateFixtures(unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    reRigidBody* p = _internalAllocator.alloc_new<reRigidBody>(&s);
    fixtures.push_back(_internalAllocator.alloc_new<reQueryable>(*p));
  }
}

void TestFixtures::clearFixtures() {
  for (reQueryable* q : fixtures) {
    _internalAllocator.alloc_delete(&q->ent);
    _internalAllocator.alloc_delete(q);
  }
  fixtures.clear();
  
  RE_EXPECT(_internalAllocator.numAllocs() == 0)
}

void TestFixtures::clearOnlyQFixtures() {
  for (reQueryable* q : fixtures) {
    _internalAllocator.alloc_delete(q);
  }
  fixtures.clear();
}

