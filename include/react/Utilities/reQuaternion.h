/**
 * @file
 * Contains the definition of the reQuaternion utility class
 */
#ifndef RE_QUATERNION_H
#define RE_QUATERNION_H

#include "react/Utilities/reMatrix.h"

/**
 * @ingroup utilities
 * Represents a quaternion
 */

struct reQuaternion {
  reQuaternion();
  reQuaternion(const reQuaternion& that);
  reQuaternion(reFloat w, reFloat x, reFloat y, reFloat z);
  
  reQuaternion& operator=(const reQuaternion& that);
  
  reQuaternion& operator+=(const reQuaternion& that);
  reQuaternion& operator-=(const reQuaternion& that);
  reQuaternion& operator*=(const reQuaternion& that);
  reQuaternion& operator*=(const reVector& that);
  reQuaternion& operator*=(reFloat s);
  
  const reQuaternion operator+(const reQuaternion& that) const;
  const reQuaternion operator-(const reQuaternion& that) const;
  const reQuaternion operator*(const reQuaternion& that) const;
  const reQuaternion operator*(const reVector& that) const;
  const reQuaternion operator*(reFloat s) const;
  
  reFloat length() const;
  reFloat lengthSq() const;
  const reMatrix toMatrix() const;
  
  union {
    struct {
      /** The real component of the quaternion */
      reFloat w;
      /** The first complex component */
      reFloat x;
      /** The second complex component */
      reFloat y;
      /** The third complex component */
      reFloat z;
    };
    struct {
      /** The quarternion as an array */
      reFloat v[4];
    };
  };
};

inline reQuaternion::reQuaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {
  // do nothing
}

inline reQuaternion::reQuaternion(const reQuaternion& that) {
  for (reUInt i = 0; i < 4; i++) {
    v[i] = that.v[i];
  }
}

inline reQuaternion::reQuaternion(reFloat w, reFloat x, reFloat y, reFloat z) : v{ w, x, y, z } {
  // do nothing
}

inline reQuaternion& reQuaternion::operator=(const reQuaternion& that) {
  for (reUInt i = 0; i < 4; i++) {
    v[i] = that.v[i];
  }
  return *this;
}

inline reQuaternion& reQuaternion::operator+=(const reQuaternion& that) {
  for (reUInt i = 0; i < 4; i++) {
    v[i] += that.v[i];
  }
  return *this;
}

inline reQuaternion& reQuaternion::operator-=(const reQuaternion& that) {
  for (reUInt i = 0; i < 4; i++) {
    v[i] -= that.v[i];
  }
  return *this;
}

inline reQuaternion& reQuaternion::operator*=(const reQuaternion& that) {
  reQuaternion tmp(*this);
  w = tmp.w * that.w - tmp.x * that.x - tmp.y * that.y - tmp.z * that.z;
  x = tmp.w * that.x + tmp.x * that.w + tmp.y * that.z - tmp.z * that.y;
  y = tmp.w * that.y - tmp.x * that.z + tmp.y * that.w + tmp.z * that.x;
  z = tmp.w * that.z + tmp.x * that.y - tmp.y * that.x + tmp.z * that.w;
  return *this;
}

inline reQuaternion& reQuaternion::operator*=(const reVector& that) {
  reQuaternion tmp(*this);
  w = - tmp.x * that.x - tmp.y * that.y - tmp.z * that.z;
  x = tmp.w * that.x + tmp.y * that.z - tmp.z * that.y;
  y = tmp.w * that.y - tmp.x * that.z + tmp.z * that.x;
  z = tmp.w * that.z + tmp.x * that.y - tmp.y * that.x;
  return *this;
}

inline reQuaternion& reQuaternion::operator*=(reFloat s) {
  for (reUInt i = 0; i < 4; i++) {
    v[i] *= s;
  }
  return *this;
}

inline const reQuaternion reQuaternion::operator+(const reQuaternion& that) const {
  return reQuaternion(*this) += that;
}

inline const reQuaternion reQuaternion::operator-(const reQuaternion& that) const {
  return reQuaternion(*this) -= that;
}

inline const reQuaternion reQuaternion::operator*(const reQuaternion& that) const {
  return reQuaternion(*this) *= that;
}

inline const reQuaternion reQuaternion::operator*(const reVector& that) const {
  return reQuaternion(*this) *= that;
}

inline const reQuaternion reQuaternion::operator*(reFloat s) const {
  return reQuaternion(*this) *= s;
}

inline reFloat reQuaternion::length() const {
  return reSqrt(lengthSq());
}

inline reFloat reQuaternion::lengthSq() const {
  return w*w + x*x + y*y + z*z;
}

#endif
