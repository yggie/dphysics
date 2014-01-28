/**
 * @file
 * Contains the definition of the re::quat utility class
 */
#ifndef RE_QUATERNION_H
#define RE_QUATERNION_H

#include "react/Math/mat3x3.h"

namespace re {

  /**
   * @ingroup maths
   * Represents a quaternion
   */
 
  struct quat {
    quat();
    quat(const quat& that);
    quat(reFloat r, reFloat i, reFloat j, reFloat k);
    
    quat& operator=(const quat& that);
    
    reFloat& operator[](reUInt i);
    const reFloat& operator[](reUInt i) const;
    
    quat& operator+=(const quat& that);
    quat& operator-=(const quat& that);
    quat& operator*=(const quat& that);
    quat& operator*=(const re::vec3& that);
    quat& operator*=(reFloat s);
    quat& operator/=(reFloat s);
    
    const quat operator+(const quat& that) const;
    const quat operator-(const quat& that) const;
    const quat operator*(const quat& that) const;
    const quat operator*(const re::vec3& that) const;
    const quat operator*(reFloat s) const;
    const quat operator/(reFloat s) const;
    
    union {
      struct {
        /** The real component of the quaternion */
        reFloat r;
        /** The first complex component */
        reFloat i;
        /** The second complex component */
        reFloat j;
        /** The third complex component */
        reFloat k;
      };
      struct {
        /** The quarternion as an array */
        reFloat v[4];
      };
    };
    
    static const quat random();
  };

  inline quat::quat() : v{ 1.0, 0.0, 0.0, 0.0 } {
    // do nothing
  }

  inline quat::quat(const quat& that) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] = that.v[i];
    }
  }

  inline quat::quat(reFloat r, reFloat i, reFloat j, reFloat k) : v{ r, i, j, k } {
    // do nothing
  }

  inline quat& quat::operator=(const quat& that) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] = that.v[i];
    }
    return *this;
  }

  inline reFloat& quat::operator[](reUInt i) {
    RE_ASSERT(i < 4)
    return v[i];
  }

  inline const reFloat& quat::operator[](reUInt i) const {
    RE_ASSERT(i < 4)
    return v[i];
  }

  inline quat& quat::operator+=(const quat& that) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] += that.v[i];
    }
    return *this;
  }

  inline quat& quat::operator-=(const quat& that) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] -= that.v[i];
    }
    return *this;
  }

  inline quat& quat::operator*=(const quat& that) {
    quat tmp(*this);
    r = tmp.r * that.r - tmp.i * that.i - tmp.j * that.j - tmp.k * that.k;
    i = tmp.r * that.i + tmp.i * that.r + tmp.j * that.k - tmp.k * that.j;
    j = tmp.r * that.j - tmp.i * that.k + tmp.j * that.r + tmp.k * that.i;
    k = tmp.r * that.k + tmp.i * that.j - tmp.j * that.i + tmp.k * that.r;
    return *this;
  }

  inline quat& quat::operator*=(const re::vec3& that) {
    quat tmp(*this);
    r = - tmp.i * that.x - tmp.j * that.y - tmp.k * that.z;
    i = tmp.r * that.x + tmp.j * that.z - tmp.k * that.y;
    j = tmp.r * that.y - tmp.i * that.z + tmp.k * that.x;
    k = tmp.r * that.z + tmp.i * that.y - tmp.j * that.x;
    return *this;
  }

  inline quat& quat::operator*=(reFloat s) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] *= s;
    }
    return *this;
  }

  inline quat& quat::operator/=(reFloat s) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] /= s;
    }
    return *this;
  }

  inline const quat quat::operator+(const quat& that) const {
    return quat(*this) += that;
  }

  inline const quat quat::operator-(const quat& that) const {
    return quat(*this) -= that;
  }

  inline const quat quat::operator*(const quat& that) const {
    return quat(*this) *= that;
  }

  inline const quat quat::operator*(const re::vec3& that) const {
    return quat(*this) *= that;
  }

  inline const quat quat::operator*(reFloat s) const {
    return quat(*this) *= s;
  }

  inline const quat quat::operator/(reFloat s) const {
    return quat(*this) /= s;
  }
  
  inline const quat quat::random() {
    return quat(reRandom() - 0.5, reRandom() - 0.5, reRandom() - 0.5, reRandom() - 0.5);
  }
}

#include "react/Math/quat_ops.h"

#endif
