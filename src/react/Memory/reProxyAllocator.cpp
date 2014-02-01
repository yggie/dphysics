#include "react/Memory/reProxyAllocator.h"

#include "react/common.h"

reProxyAllocator::reProxyAllocator(reBaseAllocator* allocator) : reAllocator(),
_allocator(allocator) {
  RE_ASSERT(allocator != nullptr)
}

reProxyAllocator::~reProxyAllocator() {
  RE_EXPECT([](reBaseAllocator& base) {
    bool leaked = base.numAllocs() != 0 || base.used() != 0;
    if (leaked) {
      printf("MEMORY LEAKS! REM. ALLOCS = %d\n", base.numAllocs());
    }
    return !leaked;
  }(*_allocator));
  
//  show();
  
  RE_ASSERT(_allocator != nullptr)
}

void* reProxyAllocator::alloc(u32 size, u8 alignment) {
  void* tmp = _allocator->alloc(size, alignment);
//  RE_DEBUG("alloc  : {#%02d-%5d}", _allocator->numAllocs(), size + alignment)
  return tmp;
}

void reProxyAllocator::dealloc(void* ptr) {
#ifdef NDEBUG
//  unsigned int uu = _allocator->used();
#endif
  _allocator->dealloc(ptr);
//  RE_DEBUG("dealloc: {#%02d-%5d}", _allocator->numAllocs(), uu - _allocator->used())
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
  printf("\n");
}

