#ifndef RE_PROXYALLOCATOR_H
#define RE_PROXYALLOCATOR_H

#include "react/memory/BaseAllocator.h"

#include <cstdio>

namespace re {
  
  /**
   * @ingroup memory
   * Implements a proxy allocator which forwards calls to a BaseAllocator. The
   * proxy allocator will harvest information about each allocation, useful for
   * debugging.
   * 
   * @see BaseAllocator
   * @see AbsAllocator
   */
  
  class ProxyAllocator : public AbsAllocator {
  public:
    ProxyAllocator(BaseAllocator* allocator);
    ~ProxyAllocator();
    
    void* alloc(u32 size, u8 alignment) override;
    void dealloc(void* ptr) override;
    
    void show();
    BaseAllocator* allocator();
    
  private:
    BaseAllocator* _allocator;
  };
  
  inline BaseAllocator* ProxyAllocator::allocator() {
    return _allocator;
  }
}

#endif
