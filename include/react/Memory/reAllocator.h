/**
 * @file
 * Contains definition of the reAllocator abstract class
 */
#ifndef RE_ABSALLOCATOR_H
#define RE_ABSALLOCATOR_H

#include <cstdint>
#include <vector>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uintptr_t uptr;

/**
 * @ingroup memory
 * Abstract allocator class, defines the interface for custom memory management
 */

class reAllocator {
public:
  /** Default constructor, does nothing */
  reAllocator();
  /** Prohibit copying */
  reAllocator(const reAllocator&) = delete;
  /** Virtual destructor, does nothing */
  virtual ~reAllocator();
  
  // custom memory allocation functions
  virtual void* alloc(u32 size, u8 alignment) = 0;
  virtual void dealloc(void* p) = 0;
  
  /** Prohibit copying */
  reAllocator& operator=(const reAllocator&) = delete;
  
  // convenient allocation functions
  template <class T> T* alloc_new();
  template <class X, class Y> X* alloc_new(Y arg);
  template <class X, class Y, class Z> X* alloc_new(Y arg1, Z arg2);
  template <class A, class B, class C, class D, class E> A* alloc_new(B arg1, C arg2, D arg3, E arg4);
  template <class T> void alloc_delete(T* ptr);
  
  void* nextAlignedAddress(void* ptr, u8 alignment) const;
  u8 alignAdjustment(void* ptr, u8 alignment) const;
  u8 alignAdjustmentWithHeader(void* ptr, u8 alignment, u8 headerSize) const;
};

inline reAllocator::reAllocator() { }
inline reAllocator::~reAllocator() { }

/**
 * @fn void* reAllocator::alloc(u32 size, u8 alignment)
 * @brief Allocates memory given the size and required alignment
 * 
 * @param size The size of the memory to allocate
 * @param alignment The required alignment
 * @return A pointer to the allocated address
 */

/**
 * @fn void reAllocator::dealloc(void* p)
 * @brief Deallocates a block of memory previously allocated by the allocator
 * 
 * @param p A pointer to the address
 */

/**
 * Allocates memory for the object using a default constructor
 * 
 * @return The allocated instance
 */

template <class T> inline T* reAllocator::alloc_new() {
  return new (alloc(sizeof(T), __alignof(T))) T();
}

/**
 * Allocates memory for the object using a constructor with a single argument
 * 
 * @param arg A single argument
 * @return The allocated instance
 */

template <class X, class Y> inline X* reAllocator::alloc_new(Y arg) {
  return new (alloc(sizeof(X), __alignof(X))) X(arg);
}

/**
 * Allocates memory for the object using a constructor with a two arguments
 * 
 * @param arg1 The first argument
 * @param arg2 The second argument
 * @return The allocated instance
 */


template <class X, class Y, class Z> inline X* reAllocator::alloc_new(Y arg1, Z arg2) {
  return new (alloc(sizeof(X), __alignof(X))) X(arg1, arg2);
}

template <class A, class B, class C, class D, class E> inline A* reAllocator::alloc_new(B arg1, C arg2, D arg3, E arg4) {
  return new (alloc(sizeof(A), __alignof(A))) A(arg1, arg2, arg3, arg4);
}

/**
 * Calls the destructor and deallocates the object referenced by the pointer
 * 
 * @param ptr The pointer to the object
 */

template <class T> inline void reAllocator::alloc_delete(T* ptr) {
  if (ptr != nullptr) {
    ptr->~T();
    dealloc(ptr);
  }
}

/**
 * Computes pointer to the next aligned address
 * 
 * @param ptr The allocated pointer
 * @param alignment The required alignment
 * @return The pointer to the next aligned address
 */

inline void* reAllocator::nextAlignedAddress(void* ptr, u8 alignment) const {
  return (void*)( ( (uptr)ptr + (alignment - 1) ) & ~(alignment - 1) );
}

/**
 * Calculates the necessary adjustment to align the pointer to the address
 * block
 * 
 * @param ptr The allocated pointer
 * @param alignment The required alignment
 * @return The necessary adjustment for alignment
 */

inline u8 reAllocator::alignAdjustment(void* ptr, u8 alignment) const {
  u8 adjustment = alignment - ( (uptr)ptr & (alignment - 1) );
  
  // already aligned
  if (adjustment == alignment) {
    return 0;
  }
  
  return adjustment;
}

/**
 * Calculates the necessary adjustment to align the pointer to the address
 * block, also adds space for headers
 * 
 * @param ptr The allocated pointer
 * @param alignment The required alignment
 * @param headerSize The size of the header
 * @return The required alignment
 */

inline u8 reAllocator::alignAdjustmentWithHeader(void* ptr, u8 alignment, u8 headerSize) const {
  u8 adjustment = alignment - ( (uptr)ptr & (alignment - 1) );
  
  // already aligned
  if (adjustment == alignment) {
    adjustment = 0;
  }
  
  u8 neededSpace = headerSize;
  
  // further adjustment is required
  if (adjustment < neededSpace) {
    neededSpace -= adjustment;
    
    // calculates the minimum adjustment to maintain alignment
    adjustment += alignment * (neededSpace / alignment);
    if (neededSpace % alignment > 0) {
      adjustment += alignment;
    }
  }
  
  return adjustment;
}

#endif
