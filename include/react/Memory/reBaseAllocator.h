/**
 * @file
 * Contains definition of the reBaseAllocator abstract class
 */
#ifndef RE_BASEALLOCATOR_H
#define RE_BASEALLOCATOR_H

#include "react/Memory/reAllocator.h"

/**
 * @ingroup memory
 * Extends the reAllocator to include memory monitoring functions
 * 
 * @see reAllocator
 */

class reBaseAllocator : public reAllocator {
public:
  /** Default constructor, does nothing */
  reBaseAllocator();
  /** Prohibit copying */
  reBaseAllocator(const reBaseAllocator&) = delete;
  /** Virtual destructor, does nothing */
  virtual ~reBaseAllocator();
  
  // memory allocation methods
  virtual void* alloc(u32 size, u8 alignment) = 0;
  virtual void dealloc(void* p) = 0;
  
  /** Prohibit copying */
  reBaseAllocator& operator=(const reBaseAllocator&) = delete;
 
  // memory monitoring functions
  virtual u32 used() const = 0;
  virtual u32 numAllocs() const = 0;
  virtual u32 size() const = 0;
  virtual void* ptr() const = 0;
};

inline reBaseAllocator::reBaseAllocator() : reAllocator() { }
inline reBaseAllocator::~reBaseAllocator() { }

/**
 * @fn u32 reBaseAllocator::used() const
 * Returns the number of bytes used
 * 
 * @return The number of bytes used
 */

/**
 * @fn u32 reBaseAllocator::numAllocs() const
 * Returns the number allocations made
 * 
 * @return The number allocations made
 */

#endif
