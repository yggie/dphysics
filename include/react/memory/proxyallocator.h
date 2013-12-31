#ifndef RE_PROXYALLOCATOR_H
#define RE_PROXYALLOCATOR_H

#include "react/memory/baseallocator.h"

namespace re {
  class ProxyAllocator : public AbsAllocator {
  public:
    ProxyAllocator(BaseAllocator* allocator);
    ~ProxyAllocator();
    
    void* alloc(u32 size, u8 alignment) override;
    void dealloc(void* ptr) override;
    
    BaseAllocator* allocator();
    
  private:
    BaseAllocator* _allocator;
  };
  
  inline BaseAllocator* ProxyAllocator::allocator() {
    return _allocator;
  }
}

#endif
