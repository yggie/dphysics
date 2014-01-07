/**
 * @file
 * Contains the definition of the reProxyAllocator class
 */
#ifndef RE_PROXYALLOCATOR_H
#define RE_PROXYALLOCATOR_H

#include "react/Memory/reBaseAllocator.h"

#include <cstdio>

/**
 * @ingroup memory
 * Implements a proxy allocator which forwards calls to a reBaseAllocator. The
 * proxy allocator will harvest information about each allocation, useful for
 * debugging.
 * 
 * @see reBaseAllocator
 * @see reAllocator
 */

class reProxyAllocator : public reAllocator {
public:
  reProxyAllocator(reBaseAllocator* allocator);
  ~reProxyAllocator();
  
  void* alloc(u32 size, u8 alignment) override;
  void dealloc(void* ptr) override;
  
  void show();
  reBaseAllocator* allocator();
  
private:
  reBaseAllocator* _allocator;
};

inline reBaseAllocator* reProxyAllocator::allocator() {
  return _allocator;
}

#endif
