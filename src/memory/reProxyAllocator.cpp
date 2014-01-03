#include "react/memory/reProxyAllocator.h"

#include "react/common.h"

#include <cstdio>

reProxyAllocator::reProxyAllocator(reBaseAllocator* allocator) : reAllocator(),
_allocator(allocator) {
  RE_ASSERT(allocator != nullptr, "The allocator cannot be null")
}

reProxyAllocator::~reProxyAllocator() {
  RE_LOG("stats  : {#%02d-%4d/%4d} NB: LEAKS", _allocator->numAllocs(), _allocator->used(), _allocator->size())
  RE_ASSERT((_allocator->used() == 0 && _allocator->numAllocs() == 0), "Memory leak detected!")
  
//  show();
  
  RE_ASSERT(_allocator != nullptr, "No allocator was used by the proxy")
  if (_allocator != nullptr) {
    delete _allocator;
  }
}

void* reProxyAllocator::alloc(u32 size, u8 alignment) {
  void* tmp = _allocator->alloc(size, alignment);
  RE_LOG("alloc  : {#%02d-%4d/%4d} NB: LEAKS", _allocator->numAllocs(), _allocator->used(), _allocator->size())
  return tmp;
}

void reProxyAllocator::dealloc(void* ptr) {
  _allocator->dealloc(ptr);
  RE_LOG("dealloc: {#%02d-%4d/%4d} NB: LEAKS", _allocator->numAllocs(), _allocator->used(), _allocator->size())
}

void reProxyAllocator::show() {
  const int size = _allocator->size();
  const u8* ptr = (u8*)_allocator->ptr();
  
  printf(" |  %02X", ptr[0]);
  for (int i = 1; i < size; i++) {
    if (i % 20 == 0) {
      printf("  |\n |  %02X", ptr[i]);
    } else {
      printf(":%02X", ptr[i]);
    }
  }
  printf("\n");}

