#ifndef RE_FREELISTALLOCATOR_H
#define RE_FREELISTALLOCATOR_H

#include "react/memory/BaseAllocator.h"

namespace re {
  class FreeListAllocator : public BaseAllocator {
  public:
    FreeListAllocator(u32 size, void* ptr);
    ~FreeListAllocator();
    
    void* alloc(u32 size, u8 alignment) override;
    void dealloc(void* p) override;
    
    u32 used() const override;
    u32 numAllocs() const override;
    u32 size() const override;
    void* ptr() const override;
    
  private:
    u32 _used;
    u32 _numAllocs;
    const u32 _size;
    void* _ptr;
    
    struct Header {
      u32 size;
      u32 adjustment;
    };
    
    struct FreeBlock {
      u32 size;
      FreeBlock* next;
    };
    
    /** prevent copying */
    FreeListAllocator(const FreeListAllocator&) : BaseAllocator(), _size(0) { }
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
  
  inline u32 FreeListAllocator::size() const {
    return _size;
  }
  
  inline void* FreeListAllocator::ptr() const {
    return _ptr;
  }
}

#endif
