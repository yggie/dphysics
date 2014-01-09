/**
 * @file
 * Contains global definitions used throughout the engine, including logging
 * and debugging functions
 */
#ifndef RE_COMMON_H
#define RE_COMMON_H

/** a customizable base floating point type */
typedef float reFloat;

typedef int reInt;

typedef unsigned int reUInt;

/** If RE_ZERO_MEMORY is defined, this value will be used to set unused memory */
#ifndef RE_ZERO_MEM_VAL
#define RE_ZERO_MEM_VAL   0
#endif

#ifdef NDEBUG
  #include <cstdio>
  #include <string.h>
  
  #define RE_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
  
  /**
   * If NDEBUG is defined, all log outputs will be forwarded to this file
   */
  #define RE_LOG_OUTPUT       stderr
  #define RE_ZERO_MEMORY
  #define __RE_FUNC__         __func__
  
  #ifndef RE_SOFT_STOP
    #define _RE_STOP_PROGRAM    __builtin_trap();
  #else
    #define _RE_STOP_PROGRAM
  #endif
  
  #define _RE_MSG(msg)                fprintf(RE_LOG_OUTPUT, "[DEBUG] %s:%d: %s", RE_FILE, __LINE__, msg)
  
  #define RE_NOT_IMPLEMENTED \
    fprintf(RE_LOG_OUTPUT, \
      "[DEBUG] %s:%d: (%s)(%s %s) %s\n", RE_FILE, __LINE__, \
      __RE_FUNC__, __DATE__, __TIME__, "Feature not implemented");
  
  #define RE_IMPOSSIBLE \
    fprintf(RE_LOG_OUTPUT, "[ALERT] %s:%d: Impossible program flow!\n", RE_FILE, __LINE__);
  
  #define RE_ASSERT(expr, msg) \
  do { \
    if(!(expr)) { \
      fprintf(RE_LOG_OUTPUT, "[FATAL] %s:%d: %s\n", RE_FILE, __LINE__, msg); \
      _RE_STOP_PROGRAM \
    } \
  } while(0);
  
  #define RE_ASSERT_WARN(expr, msg) \
    do { \
      if(!(expr)) { \
        fprintf(RE_LOG_OUTPUT, "[WARN]  %s:%d: %s\n", RE_FILE, __LINE__, msg); \
      } \
    } while(0);
  
  #define RE_LOG(...) \
    _RE_MSG(""), fprintf(RE_LOG_OUTPUT, __VA_ARGS__), fprintf(RE_LOG_OUTPUT, "\n");
    
#else
  /**
   *  Marks features which have not yet been implemented. Does nothing if DEBUG
   * is not defined
   */
  #define RE_NOT_IMPLEMENTED
  /**
   * Marks areas of the code which should be impossible to react. Does nothing
   * if NDEBUG is not defined
   */
  #define RE_IMPOSSIBLE
  /**
   * Asserts a given condition, halting the program and showing a message if it
   * fails. Does nothing if NDEBUG is not defined
   * 
   * @param expr The assert condition
   * @param msg The message shown on failure
   */
  #define RE_ASSERT(expr, msg)   do { (void)sizeof(expr); } while(0);
  /**
   * Asserts a given condition, showing a message if it fails. Does nothing if
   * NDEBUG is not defined
   * 
   * @param expr The assert condition
   * @param msg The message shown on failure
   */
  #define RE_ASSERT_WARN(expr, msg)   do { (void)sizeof(expr); } while(0);
  /**
   * Behaves like a prinf statement, but includes additional information such
   * as line number and file name. Does nothing if NDEBUG is not defined
   */
  #define RE_LOG(...)
#endif

#include "react/Memory/reBaseAllocator.h"

class reShape;
class reEnt;

namespace re {
  
  /** A single shared allocator instance to allocate memory for the engine */
  extern reAllocator* globalAllocator;

  /**
   * Uses the global allocator to allocate memory for the object using the
   * default constructor
   * 
   * @return The allocated instance
   */
  
  template <class T> T* alloc_new() {
    return new T();
//  return new (globalAllocator->alloc(sizeof(T), __alignof(T))) T();
  }

  /**
   * Uses the global allocator to allocate memory for the object using a
   * constructor with a single argument
   * 
   * @param arg A single argument
   * @return The allocated instance
   */
  
  template <class T, class Y> T* alloc_new(Y arg) {
    return new T(arg);
//    return globalAllocator->alloc_new<T, Y>(arg);
  }
  
  template <class T> void alloc_delete(T* ptr) {
    delete ptr;
//    globalAllocator->alloc_delete<T>(ptr);
  }
  
  reShape* copyOf(const reShape& shape);
  template <> void alloc_delete<reShape>(reShape* shape);
};

#endif
