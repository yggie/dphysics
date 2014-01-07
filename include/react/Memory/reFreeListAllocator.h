/**
 * @file
 * Contains the definition of the reFreeListAllocator class
 */
#ifndef RE_FREELISTALLOCATOR_H
#define RE_FREELISTALLOCATOR_H

#include "react/Memory/reBaseAllocator.h"

/**
 * @ingroup memory
 * Implements a free list memory allocator
 * 
 * @see reBaseAllocator
 */

class reFreeListAllocator : public reBaseAllocator {
public:
  reFreeListAllocator(u32 size, void* ptr);
  /** Prohibit copying */
  reFreeListAllocator(const reFreeListAllocator&) = delete;
  ~reFreeListAllocator();
  
  void* alloc(u32 size, u8 alignment) override;
  void dealloc(void* p) override;
  
  /** Prohibit copying */
  reFreeListAllocator& operator=(const reFreeListAllocator&) = delete;
  
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
  
  FreeBlock* _freeBlocks;
};

inline u32 reFreeListAllocator::used() const {
  return _used;
}

inline u32 reFreeListAllocator::numAllocs() const {
  return _numAllocs;
}

inline u32 reFreeListAllocator::size() const {
  return _size;
}

inline void* reFreeListAllocator::ptr() const {
  return _ptr;
}

#endif
