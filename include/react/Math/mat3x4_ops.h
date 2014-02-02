#ifndef RE_MAT3X4_OPS_H
#define RE_MAT3X4_OPS_H

#include "react/Utilities/reTransform.h"

using namespace re; // just for Doxygen

namespace re {
  
  inline reFloat det(const re::mat3x4& tm) {
    const reFloat m0 = tm.m[1][1]*tm.m[2][2] - tm.m[1][2]*tm.m[2][1];
    const reFloat m1 = tm.m[1][2]*tm.m[2][0] - tm.m[1][0]*tm.m[2][2];
    const reFloat m2 = tm.m[1][0]*tm.m[2][1] - tm.m[1][1]*tm.m[2][0];
    
    return tm.m[0][0]*m0 + tm.m[0][1]*m1 + tm.m[0][2]*m2;
  }
  
  /**
   * @ingroup maths
   * Computes the inverse of the matrix
   * 
   * @param tm The matrix to invert
   * @return The inverse of the input matrix
   */
  
  inline const re::mat3x4 inverse(const re::mat3x4& tm) {
    mat3x4 tmp(tm);
    
    const reFloat a = tm.v[1]*tm.m[2][2] - tm.v[2]*tm.m[1][2];
    const reFloat b = tm.v[1]*tm.m[2][1] - tm.v[2]*tm.m[1][1];
    const reFloat c = tm.v[1]*tm.m[2][0] - tm.v[2]*tm.m[1][0];
    
    tmp.m[0][0] = tm.m[1][1]*tm.m[2][2] - tm.m[1][2]*tm.m[2][1];
    tmp.m[0][1] = tm.m[0][2]*tm.m[2][1] - tm.m[0][1]*tm.m[2][2];
    tmp.m[0][2] = tm.m[0][1]*tm.m[1][2] - tm.m[0][2]*tm.m[1][1];
    tmp.v[0]    = a*tm.m[0][1] - b*tm.m[0][2] - tm.v[0]*tmp.m[0][0];
    
    tmp.m[1][0] = tm.m[1][2]*tm.m[2][0] - tm.m[1][0]*tm.m[2][2];
    tmp.m[1][1] = tm.m[0][0]*tm.m[2][2] - tm.m[0][2]*tm.m[2][0];
    tmp.m[1][2] = tm.m[0][2]*tm.m[1][0] - tm.m[0][0]*tm.m[1][2];
    tmp.v[1]    = c*tm.m[0][2] - a*tm.m[0][0] - tm.v[0]*tmp.m[1][0];
    
    tmp.m[2][0] = tm.m[1][0]*tm.m[2][1] - tm.m[1][1]*tm.m[2][0];
    tmp.m[2][1] = tm.m[0][1]*tm.m[2][0] - tm.m[0][0]*tm.m[2][1];
    tmp.m[2][2] = tm.m[0][0]*tm.m[1][1] - tm.m[0][1]*tm.m[1][0];
    tmp.v[2]    = b*tm.m[0][0] - c*tm.m[0][1] - tm.v[0]*tmp.m[2][0];
    
    const reFloat det = tm.m[0][0]*tmp.m[0][0] + tm.m[0][1]*tmp.m[1][0] +
                        tm.m[0][2]*tmp.m[2][0];
    
    tmp.m /= det;
    tmp.v /= det;
    
    return tmp;
  }
  
  /**
   * @ingroup maths
   * Returns true if the two matrices are similar
   * 
   * @param a The first matrix
   * @param b The second matrix
   * @return True if the two matrices are similar
   */
  
  inline bool similar(const mat3x4& a, const mat3x4& b) {
    return re::similar(a.v, b.v) && re::similar(a.m, b.m);
  }
}

#endif
