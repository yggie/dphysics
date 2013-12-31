#ifndef RE_COMMON_H
#define RE_COMMON_H

#ifndef PI
#define PI 3.14159265359
#endif

/** a customizable base floating point type */
typedef float reFloat;

const reFloat D_FP_TOLERANCE = 1e-5;

#ifdef DEBUG
  #include <cstdio>
  
  #ifndef RE_LOG_OUTPUT
  #define RE_LOG_OUTPUT stderr
  #endif
  
  #ifndef RE_SOFT_STOP
    #define _RE_STOP_PROGRAM    __builtin_trap();
  #else
    #define _RE_STOP_PROGRAM
  #endif
  
  #define __RE_FUNC__         __func__
  
  #define _RE_MSG(msg)        fprintf(RE_LOG_OUTPUT, "[REACT] %s:%d: %s", __FILE__, __LINE__, msg)
  #define _RE_MSGN(msg)       fprintf(RE_LOG_OUTPUT, "[REACT] %s:%d: %s\n", __FILE__, __LINE__, msg)
  #define RE_NOT_IMPLEMENTED  fprintf(RE_LOG_OUTPUT, "[REACT] %s:%d: (%s)(%s %s) %s\n", __FILE__, __LINE__, __RE_FUNC__, __DATE__, __TIME__, "Feature not implemented");
  #define RE_IMPOSSIBLE       _RE_MSG("Impossible program sequence\n");
  #define RE_ASSERT(x, msg)   do { if(!(x)) { _RE_MSGN(msg); _RE_STOP_PROGRAM } } while(0);
  #define RE_LOG(...)         _RE_MSG(""), fprintf(RE_LOG_OUTPUT, __VA_ARGS__), fprintf(RE_LOG_OUTPUT, "\n");
#else
  #define RE_NOT_IMPLEMENTED
  #define RE_IMPOSSIBLE
  #define RE_ASSERT(x, msg)   do { (void)sizeof(x); } while(0);
  #define RE_LOG(...)
#endif

#endif
