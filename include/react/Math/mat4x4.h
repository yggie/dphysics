/**
 * @file
 * Contains definition of the mat4x4 class
 */
#ifndef RE_TMATRIX_H
#define RE_TMATRIX_H

#include "react/common.h"
#include "react/Math/vec3.h"
#include "react/Math/mat3x3.h"

namespace re {
  
  /**
   * @ingroup maths
   * Defines a 4x4 matrix
   */
  
  class mat4x4 {
  public:
    /** Default constructor initializes the matrix to identity */
    mat4x4();
    mat4x4(const mat3x3& m);
    mat4x4(const mat4x4& tm);
    mat4x4(const reFloat* raw);
    mat4x4(reFloat d);
    mat4x4(const mat3x3& rotation, const vec3& translation);
    mat4x4(reFloat m11, reFloat m22, reFloat m33, reFloat m44);
    mat4x4(
      reFloat m11, reFloat m12, reFloat m13, reFloat m14,
      reFloat m21, reFloat m22, reFloat m23, reFloat m24,
      reFloat m31, reFloat m32, reFloat m33, reFloat m34,
      reFloat m41, reFloat m42, reFloat m43, reFloat m44
    );
    /** Destructor does nothing */
    ~mat4x4();
    
    reFloat* ptr();
    
    reFloat* operator[](reUInt i);
    const reFloat* operator[](reUInt i) const;
    
    mat4x4& operator*=(const mat4x4& tm);
    mat4x4& operator=(const mat4x4& tm);
    
    const mat4x4 operator*(const mat4x4& tm) const;
    
    const vec3 mult(const vec3& a, reFloat w) const;
    mat4x4& preMult(const mat4x4& tm);
    
    const vec3 multDir(const re::vec3& dir) const;
    const vec3 multPoint(const re::vec3& dir) const;
    
    mat4x4& translate(const re::vec3& v);
    mat4x4& translate(reFloat x, reFloat y, reFloat z);
    mat4x4& scale(const re::vec3& s);
    mat4x4& scale(reFloat x, reFloat y, reFloat z);
    mat4x4& rotate(reFloat angle, const re::vec3& axis);
    mat4x4& rotate(reFloat angle, reFloat x, reFloat y, reFloat z);
    
    reFloat v[16];
    
    static const mat4x4 translation(const re::vec3& v);
    static const mat4x4 translation(reFloat x, reFloat y, reFloat z);
    static const mat4x4 scaling(const re::vec3& s);
    static const mat4x4 scaling(reFloat x, reFloat y, reFloat z);
    static const mat4x4 rotation(reFloat angle, const re::vec3& axis);
    static const mat4x4 rotation(reFloat angle, reFloat x, reFloat y, reFloat z);
  };
  
  typedef mat4x4 mat4;

  inline mat4x4::mat4x4() : mat4x4(1.0) {
    // do nothing
  }

  inline mat4x4::mat4x4(const mat3x3& tm) {
    for (reUInt i = 0; i < 3; i++) {
      for (reUInt j = 0; j < 3; j++) {
        (*this)[i][j] = tm[i][j];
      }
    }
    v[3] = 0.0;
    v[7] = 0.0;
    v[11] = 0.0;
    v[12] = 0.0;
    v[13] = 0.0;
    v[14] = 0.0;
    v[15] = 1.0;
  }

  inline mat4x4::mat4x4(const mat4x4& tm) {
    for (int i = 0; i < 16; i++) {
      v[i] = tm.v[i];
    }
  }

  /**
   * Creates a transformation matrix from the raw array. This assumes row-major
   * ordering
   * 
   * @param raw The raw array
   */

  inline mat4x4::mat4x4(const reFloat* raw) {
    for (int i = 0; i < 16; i++) {
      v[i] = raw[i];
    }
  }

  inline mat4x4::mat4x4(reFloat d) : v{0} {
    v[0] = d;
    v[5] = d;
    v[10] = d;
    v[15] = d;
  }

  inline mat4x4::mat4x4(const mat3x3& rotation, const vec3& translation) {
    for (int i = 0; i < 3; i++) {
      (*this)[i][3] = translation[i];
      (*this)[3][i] = 0.0;
      for (int j = 0; j < 3; j++) {
        (*this)[i][j] = rotation[i][j];
      }
    }
    v[15] = 1.0;
  }
  
  inline mat4x4::mat4x4(reFloat m11, reFloat m22, reFloat m33, reFloat m44) : v{0.0} {
    v[0] = m11;
    v[5] = m22;
    v[10] = m33;
    v[15] = m44;
  }

  inline mat4x4::mat4x4(
    reFloat m11, reFloat m12, reFloat m13, reFloat m14,
    reFloat m21, reFloat m22, reFloat m23, reFloat m24,
    reFloat m31, reFloat m32, reFloat m33, reFloat m34,
    reFloat m41, reFloat m42, reFloat m43, reFloat m44
  ) : v{0} {
    v[0]  = m11; v[1]  = m12; v[2]  = m13; v[3]  = m14;
    v[4]  = m21; v[5]  = m22; v[6]  = m23; v[7]  = m24;
    v[8]  = m31; v[9]  = m32; v[10] = m33; v[11] = m34;
    v[12] = m41; v[13] = m42; v[14] = m43; v[15] = m44;
  }

  inline mat4x4::~mat4x4() {
    // do nothing
  }

  inline reFloat* mat4x4::ptr() {
    return &v[0];
  }

  /**
   * Accesses a row in the matrix
   * 
   * @param i The row number
   * @return A reference to the row values
   */

  inline reFloat* mat4x4::operator[](reUInt i) {
    return &v[4*i];
  }

  /**
   * Accesses a row in the matrix
   * 
   * Enforces const constraint
   * 
   * @param i The row number
   * @return A reference to the row values
   */

  inline const reFloat* mat4x4::operator[](reUInt i) const {
    return &v[4*i];
  }

  inline mat4x4& mat4x4::operator*=(const mat4x4& tm) {
    const mat4x4 tmp(*this);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        (*this)[i][j] = 0.0;
        for (int k = 0; k < 4; k++) {
          (*this)[i][j] += tmp[i][k]*tm[k][j];
        }
      }
    }
    return *this;
  }

  inline mat4x4& mat4x4::operator=(const mat4x4& tm) {
    for (int i = 0; i < 16; i++) {
      v[i] = tm.v[i];
    }
    return *this;
  }

  inline const mat4x4 mat4x4::operator*(const mat4x4& tm) const {
    return mat4x4(*this) *= tm;
  }

  inline const vec3 mat4x4::mult(const vec3& a, reFloat w) const {
    const mat4x4& m = *this;
    return vec3(
      a[0]*m[0][0] + a[1]*m[0][1] + a[2]*m[0][2] + w*m[0][3],
      a[0]*m[1][0] + a[1]*m[1][1] + a[2]*m[1][2] + w*m[1][3],
      a[0]*m[2][0] + a[1]*m[2][1] + a[2]*m[2][2] + w*m[2][3]
    );
  }

  inline mat4x4& mat4x4::preMult(const mat4x4& tm) {
    const mat4x4 tmp(*this);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        (*this)[i][j] = 0.0;
        for (int k = 0; k < 4; k++) {
          (*this)[i][j] += tmp[j][k]*tm[k][i];
        }
      }
    }
    return *this;
  }
  
  inline const vec3 mat4x4::multDir(const vec3& dir) const {
    const mat4x4& m = *this;
    return vec3(
      dir[0]*m[0][0] + dir[1]*m[0][1] + dir[2]*m[0][2],
      dir[0]*m[1][0] + dir[1]*m[1][1] + dir[2]*m[1][2],
      dir[0]*m[2][0] + dir[1]*m[2][1] + dir[2]*m[2][2]
    );
  }
  
  inline const vec3 mat4x4::multPoint(const vec3& pt) const {
    const mat4x4& m = *this;
    return vec3(
      pt[0]*m[0][0] + pt[1]*m[0][1] + pt[2]*m[0][2] + m[0][3],
      pt[0]*m[1][0] + pt[1]*m[1][1] + pt[2]*m[1][2] + m[1][3],
      pt[0]*m[2][0] + pt[1]*m[2][1] + pt[2]*m[2][2] + m[2][3]
    );
  }
  
  inline mat4x4& mat4x4::translate(const vec3& v) {
    return translate(v.x, v.y, v.z);
  }

  inline mat4x4& mat4x4::translate(reFloat x, reFloat y, reFloat z) {
    v[3] += x;
    v[7] += y;
    v[11] += z;
    return *this;
  }
  
  inline mat4x4& mat4x4::scale(const re::vec3& s) {
    return scale(s.x, s.y, s.z);
  }

  inline mat4x4& mat4x4::scale(reFloat x, reFloat y, reFloat z) {
    for (reUInt i = 0; i < 4; i++) {
      v[i + 0] *= x;
      v[i + 4] *= y;
      v[i + 8] *= z;
    }
    return *this;
  }
  
  inline mat4x4& mat4x4::rotate(reFloat angle, const vec3& axis) {
    return rotate(angle, axis.x, axis.y, axis.z);
  }

  inline mat4x4& mat4x4::rotate(reFloat angle, reFloat x, reFloat y, reFloat z) {
    return (*this) = re::mat4x4(re::mat3::rotation(angle, x, y, z)) * (*this);
  }
  
  inline const mat4x4 mat4x4::translation(const vec3& t) {
    return re::mat4x4().translate(t);
  }
  
  inline const mat4x4 mat4x4::translation(reFloat x, reFloat y, reFloat z) {
    return re::mat4x4().translate(x, y, z);
  }
  
  inline const mat4x4 mat4x4::scaling(const vec3& s) {
    return re::mat4x4().scale(s);
  }
  
  inline const mat4x4 mat4x4::scaling(reFloat x, reFloat y, reFloat z) {
    return re::mat4x4().scale(x, y, z);
  }
  
  inline const mat4x4 mat4x4::rotation(reFloat angle, const vec3& axis) {
    return re::mat4x4().rotate(angle, axis);
  }
  
  inline const mat4x4 mat4x4::rotation(reFloat angle, reFloat x, reFloat y, reFloat z) {
    return re::mat4x4().rotate(angle, x, y, z);
  }
}

#include "react/Math/mat4x4_ops.h"

#endif
