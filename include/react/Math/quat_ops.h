/**
 * @file
 * Contains the definition of mathematical operators for the re::quat class
 */
#ifndef RE_QUAT_OPS_H
#define RE_QUAT_OPS_H

#include "react/Math/quat.h"

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the squared length of the quaternion. It is considerably faster
   * than calculating the length.
   * 
   * @param q The input quaternion
   * @return The squared length of the quaternion
   */

  inline reFloat lengthSq(const re::quat& q) {
    return q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k;
  }
  
  /**
   * @ingroup maths
   * Calculates the length of the quaternion
   * 
   * @param q The input quaternion
   * @return The length of the quaternion
   */

  inline reFloat length(const re::quat& q) {
    return re::sqrt(re::lengthSq(q));
  }
  
  /**
   * @ingroup maths
   * Normalizes the given quaternion
   * 
   * @param q The quaternion to normalize
   * @return The normalized quaternion
   */

  inline const re::quat normalize(const re::quat& q) {
    return re::quat(q) /= re::length(q);
  }
  
  /**
   * Generates a random unit quaternion
   * 
   * @return A random unit quaternion
   */
  
  inline const re::quat re::quat::unit() {
    return re::normalize(re::quat::rand());
  }
  
  /**
   * @ingroup maths
   * Returns true if the two quaternions are equal within reasonable tolerance.
   * Difference is measured using the Euclidean distance metric.
   * 
   * @param a The first quaternion
   * @param b The second quaternion
   * @return True if the Euclidean distance between the quaternions is within
   * tolerance
   */
  
  inline bool similar(const re::quat& a, const re::quat& b) {
    return re::lengthSq(a - b) < re::sq(RE_FP_TOLERANCE);
  }
  
  /**
   * @ingroup maths
   * Computes the unit quaternion representing a rotation about the given axis
   * 
   * @param angle The anti-clockwise angle of rotation in radians
   * @param axis The axis of rotation
   * @return The resulting quaternion
   */
  
  inline const re::quat re::quat::rotation(reFloat angle, const vec3& axis) {
    re::quat q;
    q.r = re::cos(angle / 2.0);
    const reFloat s = re::sin(angle / 2.0);
    const reFloat L = re::length(axis);
    q.i = axis.x * s / L;
    q.j = axis.y * s / L;
    q.k = axis.z * s / L;
    return q;
  }
  
  inline const quat quat::rotation(reFloat angle, reFloat x, reFloat y, reFloat z) {
    return re::quat::rotation(angle, re::vec3(x, y, z));
  }
}

#endif
