/**
 * @file
 * Contains global definitions used throughout the engine
 */
#ifndef RE_COMMON_H
#define RE_COMMON_H

/** Default value for the mathematical constant, pi */
#ifndef PI
#define PI 3.14159265359
#endif

/** a customizable base floating point type */
typedef float reFloat;

/** tolerance for floating point values */
const reFloat D_FP_TOLERANCE = 1e-5;

/** If RE_ZERO_MEMORY is defined, this value will be used to set unused memory */
#ifndef RE_ZERO_MEM_VAL
#define RE_ZERO_MEM_VAL   0
#endif

#ifdef DEBUG
  #include <cstdio>
  
  /**
   * If DEBUG is defined, all log outputs will be forwarded to this file
   */
  static FILE* RE_LOG_OUTPUT = stderr;
  
  #ifndef RE_SOFT_STOP
    #define _RE_STOP_PROGRAM    __builtin_trap();
  #else
    #define _RE_STOP_PROGRAM
  #endif
  
  #define RE_ZERO_MEMORY
  
  #define __RE_FUNC__         __func__
  
  #define _RE_MSG(msg)                fprintf(RE_LOG_OUTPUT, "[REACT] %s:%d: %s", __FILE__, __LINE__, msg)
  
  #define RE_NOT_IMPLEMENTED \
    fprintf(RE_LOG_OUTPUT, \
      "[DEBUG] %s:%d: (%s)(%s %s) %s\n", __FILE__, __LINE__, \
      __RE_FUNC__, __DATE__, __TIME__, "Feature not implemented");
  
  #define RE_IMPOSSIBLE \
    fprintf(RE_LOG_OUTPUT, "[ALERT] %s:%d: Impossible program flow!\n", __FILE__, __LINE__);
  
  #define RE_ASSERT(expr, msg) \
  do { \
    if(!(expr)) { \
      fprintf(RE_LOG_OUTPUT, "[FATAL] %s:%d: %s\n", __FILE__, __LINE__, msg); \
      _RE_STOP_PROGRAM \
    } \
  } while(0);
  
  #define RE_ASSERT_WARN(expr, msg) \
    do { \
      if(!(expr)) { \
        fprintf(RE_LOG_OUTPUT, "[WARN]  %s:%d: %s\n", __FILE__, __LINE__, msg); \
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
   * if DEBUG is not defined
   */
  #define RE_IMPOSSIBLE
  /**
   * Asserts a given condition, halting the program and showing a message if it
   * fails. Does nothing if DEBUG is not defined
   * 
   * @param expr The assert condition
   * @param msg The message shown on failure
   */
  #define RE_ASSERT(expr, msg)   do { (void)sizeof(expr); } while(0);
  /**
   * Asserts a given condition, showing a message if it fails. Does nothing if
   * DEBUG is not defined
   * 
   * @param expr The assert condition
   * @param msg The message shown on failure
   */
  #define RE_ASSERT_WARN(expr, msg)   do { (void)sizeof(expr); } while(0);
  /**
   * Behaves like a prinf statement, but includes additional information such
   * as line number and file name. Does nothing if DEBUG is not defined
   */
  #define RE_LOG(...)
#endif

#endif
