/**
 * @file
 * Contains the definition of the re::quat utility class
 */
#ifndef RE_QUATERNION_H
#define RE_QUATERNION_H

#include "react/Utilities/reMatrix.h"

/**
 * @ingroup utilities
 * Represents a quaternion
 */

namespace re {
  struct quat {
    quat();
    quat(const quat& that);
    quat(const reMatrix& m);
    quat(reFloat angle, const reVector& axis);
    quat(reFloat r, reFloat i, reFloat j, reFloat k);
    
    quat& operator=(const quat& that);
    
    reFloat& operator[](reUInt i);
    const reFloat& operator[](reUInt i) const;
    
    quat& operator+=(const quat& that);
    quat& operator-=(const quat& that);
    quat& operator*=(const quat& that);
    quat& operator*=(const reVector& that);
    quat& operator*=(reFloat s);
    quat& operator/=(reFloat s);
    
    const quat operator+(const quat& that) const;
    const quat operator-(const quat& that) const;
    const quat operator*(const quat& that) const;
    const quat operator*(const reVector& that) const;
    const quat operator*(reFloat s) const;
    const quat operator/(reFloat s) const;
    
    void fromAxisAngle(reFloat angle, reFloat x, reFloat y, reFloat z);
    void fromAxisAngle(reFloat angle, const reVector& axis);
    reFloat length() const;
    reFloat lengthSq() const;
    quat& normalized();
    void normalize();
    const reMatrix toMatrix() const;
    void setFromMatrix(const reMatrix& m);
    
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
  };

  inline quat::quat() : v{ 1.0, 0.0, 0.0, 0.0 } {
    // do nothing
  }

  inline quat::quat(const quat& that) {
    for (reUInt i = 0; i < 4; i++) {
      v[i] = that.v[i];
    }
  }

  inline quat::quat(const reMatrix& m) {
    setFromMatrix(m);
  }

  inline quat::quat(reFloat angle, const reVector& axis) {
    fromAxisAngle(angle, axis);
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

  inline quat& quat::operator*=(const reVector& that) {
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

  inline const quat quat::operator*(const reVector& that) const {
    return quat(*this) *= that;
  }

  inline const quat quat::operator*(reFloat s) const {
    return quat(*this) *= s;
  }

  inline const quat quat::operator/(reFloat s) const {
    return quat(*this) /= s;
  }

  inline void quat::fromAxisAngle(reFloat angle, reFloat x, reFloat y, reFloat z) {
    fromAxisAngle(angle, reVector(x, y, z));
  }

  inline void quat::fromAxisAngle(reFloat angle, const reVector& axis) {
    r = reCos(angle / 2.0);
    const reFloat s = reSin(angle / 2.0);
    const reFloat L = axis.length();
    i = axis.x * s / L;
    j = axis.y * s / L;
    k = axis.z * s / L;
  }

  inline reFloat quat::length() const {
    return reSqrt(lengthSq());
  }

  inline reFloat quat::lengthSq() const {
    return r*r + i*i + j*j + k*k;
  }

  inline quat& quat::normalized() {
    normalize();
    return *this;
  }

  inline void quat::normalize() {
    (*this) /= length();
  }

  inline const reMatrix quat::toMatrix() const {
    const reFloat i2 = i*i;
    const reFloat j2 = j*j;
    const reFloat k2 = k*k;
    
    const reFloat ri = r*i;
    const reFloat rj = r*j;
    const reFloat rk = r*k;
    
    const reFloat ij = i*j;
    const reFloat ik = i*k;
    
    const reFloat jk = j*k;
    
    return reMatrix(
      1 - 2*(j2 + k2), 2*(ij - rk), 2*(ik + rj),
      2*(ij + rk), 1 - 2*(i2 + k2), 2*(jk - ri),
      2*(ik - rj), 2*(ri + jk), 1 - 2*(i2 + j2)
    );
  }

  inline void quat::setFromMatrix(const reMatrix& m) {
    r = ( m[0][0] + m[1][1] + m[2][2] + 1.0f) / 4.0f;
    i = ( m[0][0] - m[1][1] - m[2][2] + 1.0f) / 4.0f;
    j = (-m[0][0] + m[1][1] - m[2][2] + 1.0f) / 4.0f;
    k = (-m[0][0] - m[1][1] + m[2][2] + 1.0f) / 4.0f;
    if (r < 0.0f) r = 0.0f;
    if (i < 0.0f) i = 0.0f;
    if (j < 0.0f) j = 0.0f;
    if (k < 0.0f) k = 0.0f;
    r = reSqrt(r);
    i = reSqrt(i);
    j = reSqrt(j);
    k = reSqrt(k);
    if(r >= i && r >= j && r >= k) {
        r *= +1.0f;
        i *= reSign(m[2][1] - m[1][2]);
        j *= reSign(m[0][2] - m[2][0]);
        k *= reSign(m[1][0] - m[0][1]);
    } else if(i >= r && i >= j && i >= k) {
        r *= reSign(m[2][1] - m[1][2]);
        i *= +1.0f;
        j *= reSign(m[1][0] + m[0][1]);
        k *= reSign(m[0][2] + m[2][0]);
    } else if(j >= r && j >= i && j >= k) {
        r *= reSign(m[0][2] - m[2][0]);
        i *= reSign(m[1][0] + m[0][1]);
        j *= +1.0f;
        k *= reSign(m[2][1] + m[1][2]);
    } else if(k >= r && k >= i && k >= j) {
        r *= reSign(m[1][0] - m[0][1]);
        i *= reSign(m[2][0] + m[0][2]);
        j *= reSign(m[2][1] + m[1][2]);
        k *= +1.0f;
    } else {
        printf("coding error\n");
    }
    normalize();
  }
}

#endif
