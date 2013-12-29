#ifndef D_VEC_H
#define D_VEC_H

#include "math.h"

namespace d {
  struct vec {
    vec();
    vec(const vec& a);
    vec(dfloat _x, dfloat _y, dfloat _z);
    
    dfloat& operator[](int i);
    vec& operator=(const vec& a);
    vec& operator+=(const vec& a);
    
    const vec operator+(const vec& a);
    
    union {
      struct { dfloat x, y, z, w; };
      struct { dfloat v[4]; };
    };
  };
  
  inline vec::vec() : v{0.0} { }
  inline vec::vec(const vec& a) : x(a.x), y(a.y), z(a.z), w(0.0) { }
  inline vec::vec(dfloat _x, dfloat _y, dfloat _z) : x(_x), y(_y), z(_z), w(0.0) { }
  
  inline dfloat& vec::operator[](int i) {
    return v[i];
  }
  
  inline vec& vec::operator=(const vec& a) {
    for (int i = 0; i < 4; i++) {
      v[i] = a.v[i];
    }
    return *this;
  }
  
  inline vec& vec::operator+=(const vec& a) {
    for (int i = 0; i < 4; i++) {
      v[i] += a.v[i];
    }
    return *this;
  }
  
  const vec vec::operator+(const vec& a) {
    return vec(*this) += a;
  }
}

#endif
