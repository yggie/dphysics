/**
 * @file
 * This file contains the definition of the re::Transform struct
 */
#ifndef RE_TRANSFORM_H
#define RE_TRANSFORM_H

#include "react/Math/vec3.h"
#include "react/Math/mat3x3.h"

namespace re {

  /**
   * @ingroup maths
   * A sparse representation of a 4x4 matrix, suitable for modelling scaling,
   * rotation and translation matrix.
   */
 
  struct Transform {
    Transform();
    Transform(const Transform& that);
    Transform(const mat3x3& matrix, const vec3& translation);
    ~Transform();
    
    Transform& operator*=(const Transform& that);
    
    const Transform operator*(const Transform& that) const;
    
    Transform& operator=(const Transform& that);
    
    Transform& translate(const vec3& translation);
    Transform& translate(reFloat x, reFloat y, reFloat z);
    Transform& rotate(const mat3x3& rotation);
    Transform& rotate(reFloat angle, const vec3& axis);
    Transform& rotate(reFloat angle, reFloat x, reFloat y, reFloat z);
    Transform& scale(const vec3& scale);
    Transform& scale(reFloat x, reFloat y, reFloat z);
    
    const mat3x3& mat() const;
    const vec3& vec() const;
    
    Transform& inverted();
    const Transform inverse() const;
    
    const vec3 applyToPoint(const vec3& point) const;
    const vec3 applyToDir(const vec3& dir) const;
    
    /** Represents the scaling and/or rotational component */
    mat3x3 m;
    /** Represents the translational component */
    vec3 v;
  };

  inline Transform::Transform() : m(1.0), v(0.0, 0.0, 0.0) {
    // do nothing
  }

  inline Transform::Transform(const Transform& that) : m(that.m), v(that.v) {
    // do nothing
  }

  inline Transform::Transform(const mat3x3& matrix, const vec3& translation) : m(matrix), v(translation) {
    // do nothing
  }

  inline Transform::~Transform() {
    // do nothing
  }

  inline Transform& Transform::operator*=(const Transform& that) {
    v += m * that.v;
    m *= that.m;
    return *this;
  }

  inline const Transform Transform::operator*(const Transform& that) const {
    return Transform(*this) *= that;
  }

  inline Transform& Transform::operator=(const Transform& that) {
    m = that.m;
    v = that.v;
    return *this;
  }

  inline Transform& Transform::translate(const vec3& translation) {
    v += translation;
    return *this;
  }

  inline Transform& Transform::translate(reFloat x, reFloat y, reFloat z) {
    v += vec3(x, y, z);
    return *this;
  }

  inline Transform& Transform::rotate(const mat3x3& rotation) {
    m *= rotation;
    return *this;
  }

  inline Transform& Transform::rotate(reFloat angle, const vec3& a) {
    return rotate(angle, a.x, a.y, a.z);
  }

  inline Transform& Transform::rotate(reFloat angle, reFloat x, reFloat y, reFloat z) {
    const reFloat s   = re::sin(angle);
    const reFloat c   = re::cos(angle);
    const reFloat c1  = 1 - c;
    const reFloat L   = re::sqrt(x*x + y*y + z*z);
    
    x /= L; y /= L; z /= L;
    
    mat3x3 rot(
        c + c1*x*x,   c1*x*y - s*z,   c1*x*z + s*y,
      c1*x*y + s*z,     c + c1*y*y,   c1*y*z - s*x,
      c1*x*z - s*y,   c1*y*z + s*x,     c + c1*z*z
    );
    
    m = rot * m;
    v = rot * v;
    
    return *this;
  }

  inline Transform& Transform::scale(const vec3& scaling) {
    return scale(scaling.x, scaling.y, scaling.z);
  }

  inline Transform& Transform::scale(reFloat x, reFloat y, reFloat z) {
    for (reUInt i = 0; i < 3; i++) {
      m[0][i] *= x;
      m[1][i] *= y;
      m[2][i] *= z;
    }
    v[0] *= x;
    v[1] *= y;
    v[2] *= z;
    return *this;
  }

  inline const mat3x3& Transform::mat() const {
    return m;
  }

  inline const vec3& Transform::vec() const {
    return v;
  }

  inline Transform& Transform::inverted() {
    Transform tmp(*this);
    
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

  inline const Transform Transform::inverse() const {
    return Transform(*this).inverted();
  }

  inline const vec3 Transform::applyToPoint(const vec3& point) const {
    return vec3(
      m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + v[0],
      m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + v[1],
      m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + v[2]
    );
  }

  inline const vec3 Transform::applyToDir(const vec3& dir) const {
    return vec3(
      m[0][0] * dir.x + m[0][1] * dir.y + m[0][2] * dir.z,
      m[1][0] * dir.x + m[1][1] * dir.y + m[1][2] * dir.z,
      m[2][0] * dir.x + m[2][1] * dir.y + m[2][2] * dir.z
    );
  }
}

#include "react/Math/Transform_ops.h"

#endif
