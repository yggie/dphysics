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
#define RE_NOT_IMPLEMENTED  std::printf("react: Feature not implemented\n");
#define RE_IMPOSSIBLE       std::printf("react: Impossible program sequence");
#else
#define RE_NOT_IMPLEMENTED
#define RE_IMPOSSIBLE
#endif

#endif
