/**
 * @file
 * This file contains the definition of the reTransform struct
 */
#ifndef RE_TRANSFORM_H
#define RE_TRANSFORM_H

#include "react/Utilities/reVector.h"
#include "react/Utilities/reMatrix.h"

/**
 * @ingroup utilities
 * A sparse representation of a 4x4 matrix, suitable for modelling scaling,
 * rotation and translation matrix
 */

struct reTransform {
  reTransform();
  reTransform(const reTransform& that);
  ~reTransform();
  
  reTransform& operator*=(const reTransform& that);
  
  const reTransform operator*(const reTransform& that) const;
  
  reTransform& operator=(const reTransform& that);
  
  reTransform& translate(const reVector& translation);
  reTransform& translate(reFloat x, reFloat y, reFloat z);
  reTransform& rotate(const reMatrix& rotation);
  reTransform& rotate(reFloat angle, const reVector& axis);
  reTransform& rotate(reFloat angle, reFloat x, reFloat y, reFloat z);
  reTransform& scale(const reVector& scale);
  reTransform& scale(reFloat x, reFloat y, reFloat z);
  
  reTransform& inverted();
  const reTransform inverse() const;
  
  const reVector multPoint(const reVector& point) const;
  const reVector multDir(const reVector& dir) const;
  
  /** Represents the scaling and/or rotational component */
  reMatrix m;
  /** Represents the translational component */
  reVector v;
};

inline reTransform::reTransform() : m(1.0), v(0.0, 0.0, 0.0) {
  // do nothing
}

inline reTransform::reTransform(const reTransform& that) : m(that.m), v(that.v) {
  // do nothing
}

inline reTransform::~reTransform() {
  // do nothing
}

inline reTransform& reTransform::operator*=(const reTransform& that) {
  v += m * that.v;
  m *= that.m;
  return *this;
}

inline const reTransform reTransform::operator*(const reTransform& that) const {
  return reTransform(*this) *= that;
}

inline reTransform& reTransform::operator=(const reTransform& that) {
  m = that.m;
  v = that.v;
  return *this;
}

inline reTransform& reTransform::translate(const reVector& translation) {
  v += translation;
  return *this;
}

inline reTransform& reTransform::translate(reFloat x, reFloat y, reFloat z) {
  v += reVector(x, y, z);
  return *this;
}

inline reTransform& reTransform::rotate(const reMatrix& rotation) {
  m *= rotation;
  return *this;
}

inline reTransform& reTransform::rotate(reFloat angle, const reVector& a) {
  return rotate(angle, a.x, a.y, a.z);
}

inline reTransform& reTransform::rotate(reFloat angle, reFloat x, reFloat y, reFloat z) {
  const reFloat s   = reSin(angle);
  const reFloat c   = reCos(angle);
  const reFloat c1  = 1 - c;
  const reFloat L   = reSqrt(x*x + y*y + z*z);
  
  x /= L; y /= L; z /= L;
  
  m = reMatrix(
      c + c1*x*x,   c1*x*y - s*z,   c1*x*z + s*y,
    c1*x*y + s*z,     c + c1*y*y,   c1*y*z - s*x,
    c1*x*z - s*y,   c1*y*z + s*x,     c + c1*z*z
  ) * m;
  
  return *this;
}

inline reTransform& reTransform::scale(const reVector& scaling) {
  for (reUInt i = 0; i < 3; i++) {
    m[i][0] *= scaling.x;
    m[i][1] *= scaling.y;
    m[i][2] *= scaling.z;
  }
  return *this;
}

inline reTransform& reTransform::scale(reFloat x, reFloat y, reFloat z) {
  for (reUInt i = 0; i < 3; i++) {
    m[i][0] *= x;
    m[i][1] *= y;
    m[i][2] *= z;
  }
  return *this;
}

inline reTransform& reTransform::inverted() {
  reTransform tmp(*this);
  
  const reFloat a = tmp.v[1]*tmp.m[2][2] - tmp.v[2]*tmp.m[1][2];
  const reFloat b = tmp.v[1]*tmp.m[2][1] - tmp.v[2]*tmp.m[1][1];
  const reFloat c = tmp.v[1]*tmp.m[2][0] - tmp.v[2]*tmp.m[1][0];
  
  m[0][0] = tmp.m[1][1]*tmp.m[2][2] - tmp.m[1][2]*tmp.m[2][1];
  m[0][1] = tmp.m[0][2]*tmp.m[2][1] - tmp.m[0][1]*tmp.m[2][2];
  m[0][2] = tmp.m[0][1]*tmp.m[1][2] - tmp.m[0][2]*tmp.m[1][1];
  v[0]    = a*tmp.m[0][1] - b*tmp.m[0][2] - tmp.v[0]*m[0][0];
  
  m[1][0] = tmp.m[1][2]*tmp.m[2][0] - tmp.m[1][0]*tmp.m[2][2];
  m[1][1] = tmp.m[0][0]*tmp.m[2][2] - tmp.m[0][2]*tmp.m[2][0];
  m[1][2] = tmp.m[0][2]*tmp.m[1][0] - tmp.m[0][0]*tmp.m[1][2];
  v[1]    = c*tmp.m[0][2] - a*tmp.m[0][0] - tmp.v[0]*m[1][0];
  
  m[2][0] = tmp.m[1][0]*tmp.m[2][1] - tmp.m[1][1]*tmp.m[2][0];
  m[2][1] = tmp.m[0][1]*tmp.m[2][0] - tmp.m[0][0]*tmp.m[2][1];
  m[2][2] = tmp.m[0][0]*tmp.m[1][1] - tmp.m[0][1]*tmp.m[1][0];
  v[2]    = b*tmp.m[0][0] - c*tmp.m[0][1] - tmp.v[0]*m[2][0];
  
  const reFloat det = tmp.m[0][0]*m[0][0] + tmp.m[0][1]*m[1][0] +
                      tmp.m[0][2]*m[2][0];
  
  m /= det;
  v /= det;
  
  return *this;
}

inline const reTransform reTransform::inverse() const {
  return reTransform(*this).inverted();
}

inline const reVector reTransform::multPoint(const reVector& point) const {
  return reVector(
    m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + v[0],
    m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + v[1],
    m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + v[2]
  );
}

inline const reVector reTransform::multDir(const reVector& dir) const {
  return reVector(
    m[0][0] * dir.x + m[0][1] * dir.y + m[0][2] * dir.z,
    m[1][0] * dir.x + m[1][1] * dir.y + m[1][2] * dir.z,
    m[2][0] * dir.x + m[2][1] * dir.y + m[2][2] * dir.z
  );
}

#endif
