#ifndef RE_BASEALLOCATOR_H
#define RE_BASEALLOCATOR_H

#include "react/memory/AbsAllocator.h"

namespace re {
  class BaseAllocator : public AbsAllocator {
  public:
    /** Default constructor, does nothing */
    BaseAllocator();
    /** Virtual destructor, does nothing */
    virtual ~BaseAllocator();
    
    // memory allocation methods
    virtual void* alloc(u32 size, u8 alignment) = 0;
    virtual void dealloc(void* p) = 0;
   
    // memory monitoring functions
    virtual u32 used() const = 0;
    virtual u32 numAllocs() const = 0;
    virtual u32 size() const = 0;
    virtual void* ptr() const = 0;
  
  private:
    /** prevent copy */
    BaseAllocator(const BaseAllocator&) : AbsAllocator() { }
    /** prevent copy */
    BaseAllocator& operator=(const BaseAllocator&) { return *this; }
  };
  
  inline BaseAllocator::BaseAllocator() : AbsAllocator() { }
  inline BaseAllocator::~BaseAllocator() { }
  
  /**
   * @fn u32 BaseAllocator::used() const
   * Returns the number of bytes used
   * 
   * @return The number of bytes used
   */
  
  /**
   * @fn u32 BaseAllocator::numAllocs() const
   * Returns the number allocations made
   * 
   * @return The number allocations made
   */
}

#endif
