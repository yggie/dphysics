#ifndef RE_TESTS_HELPERS_H
#define RE_TESTS_HELPERS_H

#include "react/math.h"
#include "react/debug.h"
#include "react/Memory/reAllocator.h"

#include <gtest/gtest.h>
#include <exception>
#include <cstdio>

using namespace re;

struct SimpleAllocator : public reAllocator {
public:
  SimpleAllocator() : n(0) { }
  
  void* alloc(u32 size, u8) { n++; return malloc(size); }
  
  void dealloc(void* p) { n--; free(p); }
  
  unsigned long numAllocs() const { return n; }

private:
  unsigned long n;
};

static SimpleAllocator SHARED_ALLOCATOR;

const reUInt NUM_REPEATS = 50;
const reUInt NUM_SAMPLES = 1e4;
const reUInt NUM_SAMPLE_ERRORS = 10;

const re::vec3 ZERO_VEC = re::vec3(0.0, 0.0, 0.0);
const re::mat3 IDEN_MAT = re::mat3(1.0);
const re::Transform IDEN_MAT3x4 = re::Transform(IDEN_MAT, ZERO_VEC);
const re::mat4 IDEN_MAT4x4 = re::mat4(1.0);

#define ASSERT_NO_MEM_LEAKS()  ASSERT_EQ(SHARED_ALLOCATOR.numAllocs(), 0) << "should have no memory leaks"

#endif
