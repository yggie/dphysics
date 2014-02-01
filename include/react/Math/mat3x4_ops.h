#ifndef RE_MAT3X4_OPS_H
#define RE_MAT3X4_OPS_H

#include "react/Utilities/reTransform.h"

namespace re {
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
}

#endif
