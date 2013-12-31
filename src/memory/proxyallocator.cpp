#include "react/memory/proxyallocator.h"

#include "react/common.h"

using namespace re;

ProxyAllocator::ProxyAllocator(BaseAllocator* allocator) : AbsAllocator(),
_allocator(allocator) {
  RE_ASSERT(allocator != nullptr, "The allocator cannot be null")
}

ProxyAllocator::~ProxyAllocator() {
  RE_LOG("Used Memory: %d, Num Allocs: %d", _allocator->used(), _allocator->numAllocs())
  RE_ASSERT((_allocator->used() == 0 && _allocator->numAllocs() == 0), "Memory leak detected!")
}

void* ProxyAllocator::alloc(u32 size, u8 alignment) {
  void* tmp = _allocator->alloc(size, alignment);
  RE_LOG("alloc: {used: %d, numAllocs: %d}", _allocator->used(), _allocator->numAllocs())
  return tmp;
}

void ProxyAllocator::dealloc(void* ptr) {
  _allocator->dealloc(ptr);
  RE_LOG("dealloc: {used: %d, numAllocs: %d}", _allocator->used(), _allocator->numAllocs())
}

