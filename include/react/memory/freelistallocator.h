#ifndef RE_FREELISTALLOCATOR_H
#define RE_FREELISTALLOCATOR_H

#include "react/memory/baseallocator.h"

namespace re {
  class FreeListAllocator : public BaseAllocator {
  public:
    FreeListAllocator(u32 size, void* ptr);
    ~FreeListAllocator();
    
    void* alloc(u32 size, u8 alignment) override;
    void dealloc(void* p) override;
    
    u32 used() const override;
    u32 numAllocs() const override;
    
  private:
    u32 _used;
    u32 _numAllocs;
    
    struct Header {
      u32 size;
      u32 adjustment;
    };
    
    struct FreeBlock {
      u32 size;
      FreeBlock* next;
    };
    
    /** prevent copying */
    FreeListAllocator(const FreeListAllocator&) : BaseAllocator() { }
    /** prevent copying */
    FreeListAllocator& operator=(const FreeListAllocator&) { return *this; }
    
    FreeBlock* _freeBlocks;
  };
  
  inline u32 FreeListAllocator::used() const {
    return _used;
  }
  
  inline u32 FreeListAllocator::numAllocs() const {
    return _numAllocs;
  }
}

#endif
