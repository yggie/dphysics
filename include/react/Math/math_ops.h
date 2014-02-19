/**
 * @file
 * Contains the definitions of all math operations on custom structs
 */
#ifndef RE_MATH_OPS_H
#define RE_MATH_OPS_H

#include "react/Math/vec3.h"
#include "react/Math/quat.h"
#include "react/Math/mat3x3.h"

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the outer product between two vectors.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting matrix
   */
  
  inline const mat3x3 outer(const vec3& a, const vec3& b) {
    return mat3x3(
      a[0]*b[0], a[0]*b[1], a[0]*b[2],
      a[1]*b[0], a[1]*b[1], a[1]*b[2],
      a[2]*b[0], a[2]*b[1], a[2]*b[2]
    );
  }
  
  /**
   * @ingroup maths
   * Constructs the skew matrix of a vector.
   * 
   * @param v The vector to skew
   * @return The skew matrix
   */
  
  inline const mat3x3 skew(const vec3& v) {
    return mat3x3(
          0, -v[2],  v[1],
       v[2],     0, -v[0],
      -v[1],  v[0],     0
    );
  }
  
  inline const mat3x3 operator*(reFloat s, const mat3x3& m) {
    return m * s;
  }
  
  inline const vec3 operator*(reFloat s, const vec3& v) {
    return v * s;
  }
  
  inline const quat operator*(reFloat s, const quat& q) {
    return q * s;
  }

  inline const quat operator*(const vec3& v, const quat& q) {
    return quat(
      -v.x * q.i - v.y * q.j - v.z * q.k,
      v.x * q.r + v.y * q.k - v.z * q.j,
      -v.x * q.k + v.y * q.r + v.z * q.i,
      v.x * q.j - v.y * q.i + v.z * q.r
    );
  }
  
  /**
   * Computes the rotation matrix based on the given axis and angle of rotation
   * 
   * @param angle The anti-clockwise angle of rotation in radians
   * @param axis The axis of rotation
   * @return The 3x3 rotation matrix
   */

  inline const mat3x3 mat3x3::rotation(reFloat angle, const vec3& axis) {
    const reFloat c = re::cos(angle);
    const reFloat s = re::sin(angle);
    const vec3 a = normalize(axis);
    return mat3x3(c) + (1 - c) * outer(a, a) + skew(a) * s;
  }
  
  /**
   * Constructs a 3x3 rotation matrix from a given quaternion
   * 
   * @param q The unit quaternion
   * @return The 3x3 rotation matrix
   */

  inline const mat3x3 toMat(const quat& q) {
    const reFloat i2 = q.i*q.i;
    const reFloat j2 = q.j*q.j;
    const reFloat k2 = q.k*q.k;
    const reFloat ri = q.r*q.i;
    const reFloat rj = q.r*q.j;
    const reFloat rk = q.r*q.k;
    const reFloat ij = q.i*q.j;
    const reFloat ik = q.i*q.k;
    const reFloat jk = q.j*q.k;
    
    mat3x3 m;
    m.e[0] = 1 - 2*(j2 + k2);
    m.e[1] = 2*(ij - rk);
    m.e[2] = 2*(ik + rj);
    
    m.e[3] = 2*(ij + rk);
    m.e[4] = 1 - 2*(i2 + k2);
    m.e[5] = 2*(jk - ri);
    
    m.e[6] = 2*(ik - rj);
    m.e[7] = 2*(jk + ri);
    m.e[8] = 1 - 2*(i2 + j2);
    
    return m /= lengthSq(q);
  }
  
  inline const re::Transform toTransform(const mat4x4& m) {
    re::Transform tm;
    for (reUInt i = 0; i < 3; i++) {
      for (reUInt j = 0; j < 3; j++) {
        tm.m[i][j] = m[i][j];
      }
      tm.v[i] = m[i][3];
    }
    return tm;
  }
  
  /**
   * Constructs a unit quaternion using the given rotation matrix
   * 
   * @param m A 3x3 rotation matrix
   */

  inline const quat toQuat(const mat3x3& m) {
    quat q;
    const float tr = trace(m);
    if (tr > 0) {
      float S = 2.0 * re::sqrt(tr + 1.0);
      q.r = 0.25 * S;
      q.i = (m[2][1] - m[1][2]) / S;
      q.j = (m[0][2] - m[2][0]) / S; 
      q.k = (m[1][0] - m[0][1]) / S; 
    } else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) { 
      float S = 2.0 * re::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]); 
      q.r = (m[2][1] - m[1][2]) / S;
      q.i = 0.25 * S;
      q.j = (m[0][1] + m[1][0]) / S; 
      q.k = (m[0][2] + m[2][0]) / S; 
    } else if (m[1][1] > m[2][2]) { 
      float S = 2.0 * re::sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]);
      q.r = (m[0][2] - m[2][0]) / S;
      q.i = (m[0][1] + m[1][0]) / S; 
      q.j = 0.25 * S;
      q.k = (m[1][2] + m[2][1]) / S; 
    } else { 
      float S = 2.0 * re::sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]);
      q.r = (m[1][0] - m[0][1]) / S;
      q.i = (m[0][2] + m[2][0]) / S;
      q.j = (m[1][2] + m[2][1]) / S;
      q.k = 0.25 * S;
    }
    
    return re::normalize(q);
  }

  inline const re::mat3 orientY(const re::vec3& dir, const re::vec3& up) {
    re::vec3 v = re::normalize(dir);
    re::vec3 upn = re::normalize(up);
    if (re::abs(re::dot(v, upn) - 1.0) < RE_FP_TOLERANCE) {
      upn = re::vec3::unit();
    }
    const re::vec3 u = re::normalize(re::cross(v, upn));
    const re::vec3 w = re::normalize(re::cross(u, v));
    return re::mat3(
      u[0], v[0], w[0],
      u[1], v[1], w[1],
      u[2], v[2], w[2]
    );
  }
}

#endif
