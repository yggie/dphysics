/**
 * @file
 * Contains definition for mathematical operations using re::mat3x3
 */
#ifndef RE_MAT3_OPS_H
#define RE_MAT3_OPS_H

#include "react/Math/mat3x3.h"

using namespace re; // sadly, Doxygen is not smart enought

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the transpose of a 3x3 matrix.
   * 
   * @param a The matrix operand
   * @return The resulting matrix
   */
  
  inline const mat3x3 transpose(const mat3x3& m) {
    return mat3x3(
      m[0][0], m[1][0], m[2][0],
      m[0][1], m[1][1], m[2][1],
      m[0][2], m[1][2], m[2][2]
    );
  }
  
  /**
   * @ingroup maths
   * Calculates the determinant of a 3x3 matrix.
   * 
   * @param a The matrix operand
   * @return The resulting scalar
   */
  
  inline reFloat determinant(const mat3x3& m) {
    return m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1]) -
           m[0][1]*(m[1][0]*m[2][2] - m[1][2]*m[2][0]) +
           m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);
  }
  
  /**
   * @ingroup maths
   * Calculates the inverse of a 3x3 matrix.
   * 
   * @param a The matrix operand
   * @return The resulting matrix
   */
  
  inline const mat3x3 inverse(const mat3x3& m) {
    return mat3x3(
      m[1][1]*m[2][2] - m[1][2]*m[2][1],
      m[0][2]*m[2][1] - m[0][1]*m[2][2],
      m[0][1]*m[1][2] - m[0][2]*m[1][1],
      
      m[1][2]*m[2][0] - m[1][0]*m[2][2],
      m[0][0]*m[2][2] - m[0][2]*m[2][0],
      m[0][2]*m[1][0] - m[0][0]*m[1][2],
      
      m[1][0]*m[2][1] - m[1][1]*m[2][0],
      m[0][1]*m[2][0] - m[0][0]*m[2][1],
      m[0][0]*m[1][1] - m[0][1]*m[1][0]
    ) /= determinant(m);
  }
  
  /**
   * @ingroup maths
   * Calculates the trace of a 3x3 matrix.
   * 
   * @param a The matrix operand
   * @return The resulting scalar
   */
  
  inline reFloat trace(const mat3x3& m) {
    return m[0][0] + m[1][1] + m[2][2];
  }
  
  /**
   * @ingroup maths
   * Returns true if the two matrices are the same, within reasonable
   * tolerances. Difference is calculated using the ??? distance metric.
   * 
   * @param a The first matrix
   * @param b The second matrix
   * @return True if the maximum ??? distance is within tolerance
   */
  
  inline bool similar(const mat3x3& a, const mat3x3& b) {
    for (reUInt i = 0; i < 3; i++) {
      for (reUInt j = 0; j < 3; j++) {
        if (re::abs(a[i][j] - b[i][j]) > RE_FP_TOLERANCE) {
          return false;
        }
      }
    }
    return true;
  }
  
  inline const mat3x3 facing(const vec3& dir, const vec3& up) {
    const re::vec3& w = re::normalize(-dir);
    const re::vec3& u = re::normalize(re::cross(up, w));
    const re::vec3& v = re::normalize(re::cross(w, u));
    
    return re::mat3x3(
	    u[0], u[1], u[2],
	    v[0], v[1], v[2],
	    w[0], w[1], w[2]
    );
  }
}

#endif
