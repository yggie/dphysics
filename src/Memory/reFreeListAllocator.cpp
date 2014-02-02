#include "react/Memory/reFreeListAllocator.h"

#include <cstring>

#include "react/common.h"

/**
 * Creates a new allocator with the given size and pointer
 * 
 * @param size The size of the allocated memory pool
 * @param start The pointer to the start of the memory pool
 */

reFreeListAllocator::reFreeListAllocator(u32 size, void* start) : reBaseAllocator(),
_used(0), _numAllocs(0), _size(size), _ptr(start),
_freeBlocks((FreeBlock*)start) {
  RE_ASSERT_MSG(size > sizeof(FreeBlock), "Allocated memory is too small!")
  
#ifdef RE_ZERO_MEMORY
  memset(start, RE_ZERO_MEM_VAL, size);
#endif
  
  _freeBlocks->size = size;
  _freeBlocks->next = nullptr;
}

reFreeListAllocator::~reFreeListAllocator() {
  _freeBlocks = nullptr;
}

void* reFreeListAllocator::alloc(u32 size, u8 alignment) {
  RE_ASSERT(size != 0)
  
  // check free blocks
  FreeBlock* prevFreeBlock = nullptr;
  FreeBlock* freeBlock = _freeBlocks;
  
  while (freeBlock) {
    // calculate adjustment needed to keep object correctly aligned
    u8 adjustment = alignAdjustmentWithHeader(freeBlock, alignment, sizeof(Header));
    
    // skip block if it is too small
    if (size + adjustment > freeBlock->size) {
      prevFreeBlock = freeBlock;
      freeBlock = prevFreeBlock->next;
      continue;
    }
    
    // if we ran out of memory
    if (freeBlock->size - size - adjustment <= sizeof(Header)) {
      // increases allocation size instead of creating a new block
      size = freeBlock->size;
      
      if (prevFreeBlock != nullptr) {
        prevFreeBlock->next = freeBlock->next;
      } else {
        _freeBlocks = freeBlock->next;
      }
    } else {
      // create a new free block from the remaining memory
      FreeBlock* nextBlock = (FreeBlock*)( (uptr)freeBlock + size + adjustment);
      nextBlock->size = freeBlock->size - size - adjustment;
      nextBlock->next = freeBlock->next;
      
      if (prevFreeBlock != nullptr) {
        prevFreeBlock->next = nextBlock;
      } else {
        _freeBlocks = nextBlock;
      }
    }
    
    uptr alignedAddress = (uptr)freeBlock + adjustment;
    
    Header* header = (Header*)(alignedAddress - sizeof(Header));
    header->size = size + adjustment;
    header->adjustment = adjustment;
    
    _used += header->size;
    _numAllocs++;
    
    return (void*)alignedAddress;
  }
  
  RE_WARN("Couldn\'t find a free memory block large enough!\n")
  
  return nullptr;
}

void reFreeListAllocator::dealloc(void* ptr) {
  Header* header = (Header*)((uptr)ptr - sizeof(Header));
  
  u32 size = header->size;
  
  uptr blockStart = (uptr)ptr - header->adjustment;
  uptr blockEnd = blockStart + size;
  u32 blockSize = size;
  
#ifdef RE_ZERO_MEMORY
//  memset((void*)blockStart, RE_ZERO_MEM_VAL, blockSize);
#endif
  
  bool blockMerged = false;
  
  bool search = true;
  
  while (search) {
    search = false;
    
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = _freeBlocks;
    
    while (freeBlock != nullptr) {
      if ((uptr)freeBlock + freeBlock->size == blockStart) {
        freeBlock->size += blockSize;
        
        blockStart = (uptr)freeBlock;
        blockEnd = blockStart + freeBlock->size;
        blockSize = freeBlock->size;
        
        search = true;
        blockMerged = true;
        break;
      } else if (blockEnd == (uptr)freeBlock) {
        FreeBlock* newFreeBlock = (FreeBlock*) blockStart;
        newFreeBlock->next = freeBlock->next;
        newFreeBlock->size = blockSize + freeBlock->size;
        
        if (freeBlock == _freeBlocks) {
          _freeBlocks = newFreeBlock;
        } else if (prevFreeBlock != newFreeBlock) {
          prevFreeBlock->next = newFreeBlock;
        }
        
        blockStart = (uptr)newFreeBlock;
        blockEnd = blockStart + newFreeBlock->size;
        blockSize = newFreeBlock->size;
        
        search = true;
        blockMerged = true;
        break;
      }
      
      prevFreeBlock = freeBlock;
      freeBlock = freeBlock->next;
    }
  }
  
  if (!blockMerged) {
    FreeBlock* block = (FreeBlock*)((uptr)ptr - header->adjustment);
    block->size = blockSize;
    block->next = _freeBlocks;
    
    _freeBlocks = block;
  }
  
  _numAllocs--;
  _used -= size;
}

