/**
 * @file
 * Contains the operations related to re::vec3 alone
 */
#ifndef RE_VEC_OPS_H
#define RE_VEC_OPS_H

#include "react/Math/vec3.h"

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the cross product between two vectors.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting vector
   */
   
  inline const re::vec3 cross(const re::vec3& a, const re::vec3& b) {
    return re::vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
  }
  
  /**
   * @ingroup maths
   * Calculates the dot product between two vectors.
   * 
   * @param a The LHS vector operand
   * @param b The RHS vector operand
   * @return The resulting scalar
   */
  
  inline reFloat dot(const re::vec3& a, const re::vec3& b) {
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
  
  inline reFloat inner(const re::vec3& a, const re::vec3& b) {
    return re::dot(a, b);
  }
  
  /**
   * @ingroup maths
   * Calculates the squared length of a vector. It is faster to compute than the
   * length.
   * 
   * @param a The vector operand
   * @return The resulting scalar
   */
  
  inline reFloat lengthSq(const re::vec3& a) {
    return re::dot(a, a);
  }
  
  /**
   * @ingroup maths
   * Calculates the length of a vector.
   * 
   * @param a The vector operand
   * @return The resulting scalar
   */
  
  inline reFloat length(const re::vec3& a) {
    return re::sqrt(lengthSq(a));
  }
  
  /**
   * @ingroup maths
   * Calculates the normalized vector for the input vector.
   * 
   * @param a The vector operand
   * @return The resulting vector
   */
  
  inline re::vec3 normalize(const re::vec3& a) {
    return re::vec3(a) /= re::length(a);
  }
  
  /**
   * Generates a random unit vector
   * 
   * @return A random unit vector
   */
  
  inline const re::vec3 re::vec3::unit() {
    return re::normalize(re::vec3::rand());
  }
  
  /**
   * @ingroup maths
   * Returns true if the two vectors are within floating point tolerance.
   * Difference is calculated using the Euclidean distance metric.
   * 
   * @param a The first vector
   * @param b The second vector
   * @return True if the Euclidean distance between the two vectors is
   * negligible
   */
  
  inline bool similar(const re::vec3& a, const re::vec3& b) {
    return re::lengthSq(a - b) < re::sq(RE_FP_TOLERANCE);
  }
}

#endif
