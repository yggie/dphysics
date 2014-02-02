#ifndef RE_MAT4x4_OPS_H
#define RE_MAT4x4_OPS_H

#include "react/Math/mat4x4.h"

using namespace re; // for Doxygen

namespace re {
  inline const mat4x4 facing4(const vec3& dir, const vec3& up) {
    return re::mat4x4(re::facing(dir, up));
  }
  
  inline const mat4x4 lookat(const vec3& eye, const vec3& center, const vec3& up) {
    const re::vec3 w = re::normalize(eye - center);
    const re::vec3 u = re::normalize(re::cross(up, w));
    const re::vec3 v = re::normalize(re::cross(w, u));
    
    return re::mat4x4(
		    u[0], u[1], u[2], -re::dot(eye, u),
		    v[0], v[1], v[2], -re::dot(eye, v),
		    w[0], w[1], w[2], -re::dot(eye, w),
		       0,    0,    0,        1
    );
  }
  
  inline const mat4x4 inverse(const mat4x4& m) {
    reFloat inv[16];

    inv[0] = m.v[5]  * m.v[10] * m.v[15] - 
             m.v[5]  * m.v[11] * m.v[14] - 
             m.v[9]  * m.v[6]  * m.v[15] + 
             m.v[9]  * m.v[7]  * m.v[14] +
             m.v[13] * m.v[6]  * m.v[11] - 
             m.v[13] * m.v[7]  * m.v[10];

    inv[4] = -m.v[4]  * m.v[10] * m.v[15] + 
              m.v[4]  * m.v[11] * m.v[14] + 
              m.v[8]  * m.v[6]  * m.v[15] - 
              m.v[8]  * m.v[7]  * m.v[14] - 
              m.v[12] * m.v[6]  * m.v[11] + 
              m.v[12] * m.v[7]  * m.v[10];

    inv[8] = m.v[4]  * m.v[9] * m.v[15] - 
             m.v[4]  * m.v[11] * m.v[13] - 
             m.v[8]  * m.v[5] * m.v[15] + 
             m.v[8]  * m.v[7] * m.v[13] + 
             m.v[12] * m.v[5] * m.v[11] - 
             m.v[12] * m.v[7] * m.v[9];

    inv[12] = -m.v[4]  * m.v[9] * m.v[14] + 
               m.v[4]  * m.v[10] * m.v[13] +
               m.v[8]  * m.v[5] * m.v[14] - 
               m.v[8]  * m.v[6] * m.v[13] - 
               m.v[12] * m.v[5] * m.v[10] + 
               m.v[12] * m.v[6] * m.v[9];

    inv[1] = -m.v[1]  * m.v[10] * m.v[15] + 
              m.v[1]  * m.v[11] * m.v[14] + 
              m.v[9]  * m.v[2] * m.v[15] - 
              m.v[9]  * m.v[3] * m.v[14] - 
              m.v[13] * m.v[2] * m.v[11] + 
              m.v[13] * m.v[3] * m.v[10];

    inv[5] = m.v[0]  * m.v[10] * m.v[15] - 
             m.v[0]  * m.v[11] * m.v[14] - 
             m.v[8]  * m.v[2] * m.v[15] + 
             m.v[8]  * m.v[3] * m.v[14] + 
             m.v[12] * m.v[2] * m.v[11] - 
             m.v[12] * m.v[3] * m.v[10];

    inv[9] = -m.v[0]  * m.v[9] * m.v[15] + 
              m.v[0]  * m.v[11] * m.v[13] + 
              m.v[8]  * m.v[1] * m.v[15] - 
              m.v[8]  * m.v[3] * m.v[13] - 
              m.v[12] * m.v[1] * m.v[11] + 
              m.v[12] * m.v[3] * m.v[9];

    inv[13] = m.v[0]  * m.v[9] * m.v[14] - 
              m.v[0]  * m.v[10] * m.v[13] - 
              m.v[8]  * m.v[1] * m.v[14] + 
              m.v[8]  * m.v[2] * m.v[13] + 
              m.v[12] * m.v[1] * m.v[10] - 
              m.v[12] * m.v[2] * m.v[9];

    inv[2] = m.v[1]  * m.v[6] * m.v[15] - 
             m.v[1]  * m.v[7] * m.v[14] - 
             m.v[5]  * m.v[2] * m.v[15] + 
             m.v[5]  * m.v[3] * m.v[14] + 
             m.v[13] * m.v[2] * m.v[7] - 
             m.v[13] * m.v[3] * m.v[6];

    inv[6] = -m.v[0]  * m.v[6] * m.v[15] + 
              m.v[0]  * m.v[7] * m.v[14] + 
              m.v[4]  * m.v[2] * m.v[15] - 
              m.v[4]  * m.v[3] * m.v[14] - 
              m.v[12] * m.v[2] * m.v[7] + 
              m.v[12] * m.v[3] * m.v[6];

    inv[10] = m.v[0]  * m.v[5] * m.v[15] - 
              m.v[0]  * m.v[7] * m.v[13] - 
              m.v[4]  * m.v[1] * m.v[15] + 
              m.v[4]  * m.v[3] * m.v[13] + 
              m.v[12] * m.v[1] * m.v[7] - 
              m.v[12] * m.v[3] * m.v[5];

    inv[14] = -m.v[0]  * m.v[5] * m.v[14] + 
               m.v[0]  * m.v[6] * m.v[13] + 
               m.v[4]  * m.v[1] * m.v[14] - 
               m.v[4]  * m.v[2] * m.v[13] - 
               m.v[12] * m.v[1] * m.v[6] + 
               m.v[12] * m.v[2] * m.v[5];

    inv[3] = -m.v[1] * m.v[6] * m.v[11] + 
              m.v[1] * m.v[7] * m.v[10] + 
              m.v[5] * m.v[2] * m.v[11] - 
              m.v[5] * m.v[3] * m.v[10] - 
              m.v[9] * m.v[2] * m.v[7] + 
              m.v[9] * m.v[3] * m.v[6];

    inv[7] = m.v[0] * m.v[6] * m.v[11] - 
             m.v[0] * m.v[7] * m.v[10] - 
             m.v[4] * m.v[2] * m.v[11] + 
             m.v[4] * m.v[3] * m.v[10] + 
             m.v[8] * m.v[2] * m.v[7] - 
             m.v[8] * m.v[3] * m.v[6];

    inv[11] = -m.v[0] * m.v[5] * m.v[11] + 
               m.v[0] * m.v[7] * m.v[9] + 
               m.v[4] * m.v[1] * m.v[11] - 
               m.v[4] * m.v[3] * m.v[9] - 
               m.v[8] * m.v[1] * m.v[7] + 
               m.v[8] * m.v[3] * m.v[5];

    inv[15] = m.v[0] * m.v[5] * m.v[10] - 
              m.v[0] * m.v[6] * m.v[9] - 
              m.v[4] * m.v[1] * m.v[10] + 
              m.v[4] * m.v[2] * m.v[9] + 
              m.v[8] * m.v[1] * m.v[6] - 
              m.v[8] * m.v[2] * m.v[5];

    reFloat det = m.v[0] * inv[0] + m.v[1] * inv[4] + m.v[2] * inv[8] + m.v[3] * inv[12];
    det = (det == 0.0) ? 1.0 / RE_FP_TOLERANCE : 1.0 / det;
    for (int i = 0; i < 16; i++){
      inv[i] *= det;
    }
    
    return re::mat4x4(&inv[0]);
  }
  
  inline bool similar(const mat4x4& a, const mat4x4& b) {
    for (reUInt i = 0; i < 4; i++) {
      for (reUInt j = 0; j < 4; j++) {
        if (re::abs(a[i][j] - b[i][j]) > RE_FP_TOLERANCE) {
          return false;
        }
      }
    }
    return true;
  }
  
  inline const mat4x4 transpose(const mat4x4& m) {
    return re::mat4x4(
      m[0][0], m[1][0], m[2][0], m[3][0],
      m[0][1], m[1][1], m[2][1], m[3][1],
      m[0][2], m[1][2], m[2][2], m[3][2],
      m[0][3], m[1][3], m[2][3], m[3][3]
    );
  }
}

#endif
