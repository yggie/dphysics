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
}

#endif
