/**
 * @file
 * Contains the operations related to re::vec3 alone
 */
#ifndef RE_VEC_OPS_H
#define RE_VEC_OPS_H

#include "react/Math/vec3.h"

using namespace re; // sadly, Doxygen is not smart enought

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the cross product between two vectors.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting vector
   */
   
  inline const vec3 cross(const vec3& a, const vec3& b) {
    return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  }
  
  /**
   * @ingroup maths
   * Calculates the dot product between two vectors.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting scalar
   */
  
  inline reFloat dot(const vec3& a, const vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
  }
  
  /**
   * @ingroup maths
   * Calculates the inner product between two vectors. An alias to dot product.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting scalar
   */
  
  inline reFloat inner(const vec3& a, const vec3& b) {
    return dot(a, b);
  }
  
  /**
   * @ingroup maths
   * Calculates the squared length of a vector. It is faster to compute than the
   * length.
   * 
   * @param a The vector operand
   * @return The resulting scalar
   */
  
  inline reFloat lengthSq(const vec3& a) {
    return dot(a, a);
  }
  
  /**
   * @ingroup maths
   * Calculates the length of a vector.
   * 
   * @param a The vector operand
   * @return The resulting scalar
   */
  
  inline reFloat length(const vec3& a) {
    return reSqrt(lengthSq(a));
  }
  
  /**
   * @ingroup maths
   * Calculates the normalized vector for the input vector.
   * 
   * @param a The vector operand
   * @return The resulting vector
   */
  
  inline vec3 normalize(const vec3& a) {
    return vec3(a) /= length(a);
  }
}

#endif
