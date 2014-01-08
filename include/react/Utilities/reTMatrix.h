/**
 * @file
 * Contains definition of the reTMatrix class
 */
#ifndef RE_TMATRIX_H
#define RE_TMATRIX_H

#include "react/common.h"
#include "react/Utilities/reVector.h"
#include "react/Utilities/reMatrix.h"

/**
 * Defines a transformation matrix in sparse matrix form.
 */

class reTMatrix {
public:
  /** Default constructor initializes the matrix to identity */
  reTMatrix();
  reTMatrix(const reTMatrix& tm);
  reTMatrix(const reFloat* raw);
  reTMatrix(reFloat d);
  reTMatrix(const reMatrix& rotation, const reVector& translation);
  reTMatrix(
    reFloat m11, reFloat m12, reFloat m13, reFloat m14,
    reFloat m21, reFloat m22, reFloat m23, reFloat m24,
    reFloat m31, reFloat m32, reFloat m33, reFloat m34,
    reFloat m41, reFloat m42, reFloat m43, reFloat m44
  );
  /** Destructor does nothing */
  ~reTMatrix();
  
  reFloat* ptr();
  
  reFloat* operator[](int i);
  const reFloat* operator[](int i) const;
  
  reTMatrix& operator*=(const reTMatrix& tm);
  reTMatrix& operator=(const reTMatrix& tm);
  
  const reTMatrix operator*(const reTMatrix& tm) const;
  
  const reVector mult(const reVector& a, reFloat w) const;
  reTMatrix& preMult(const reTMatrix& tm);
  const reVector preMult(const reVector& a, reFloat w) const;
  
  reTMatrix& translate(reFloat x, reFloat y, reFloat z);
  reTMatrix& scale(reFloat x, reFloat y, reFloat z);
  reTMatrix& rotate(reFloat angle, reFloat x, reFloat y, reFloat z);
  
  const reTMatrix inverse() const;
  
  reFloat v[16];
};

inline reTMatrix::reTMatrix() : reTMatrix(1.0) {
  // do nothing
}

inline reTMatrix::reTMatrix(const reTMatrix& tm) {
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

inline reTMatrix::reTMatrix(const reFloat* raw) {
  for (int i = 0; i < 16; i++) {
    v[i] = raw[i];
  }
}

inline reTMatrix::reTMatrix(reFloat d) : v{0} {
  v[0] = d;
  v[5] = d;
  v[10] = d;
  v[15] = d;
}

inline reTMatrix::reTMatrix(const reMatrix& rotation, const reVector& translation) {
  for (int i = 0; i < 3; i++) {
    (*this)[i][3] = translation[i];
    (*this)[3][i] = 0.0;
    for (int j = 0; j < 3; j++) {
      (*this)[i][j] = rotation[i][j];
    }
  }
  v[15] = 1.0;
}

inline reTMatrix::reTMatrix(
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

inline reTMatrix::~reTMatrix() {
  // do nothing
}

inline reFloat* reTMatrix::ptr() {
  return &v[0];
}

/**
 * Accesses a row in the matrix
 * 
 * @param i The row number
 * @return A reference to the row values
 */

inline reFloat* reTMatrix::operator[](int i) {
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

inline const reFloat* reTMatrix::operator[](int i) const {
  return &v[4*i];
}

inline reTMatrix& reTMatrix::operator*=(const reTMatrix& tm) {
  const reTMatrix tmp(*this);
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

inline reTMatrix& reTMatrix::operator=(const reTMatrix& tm) {
  for (int i = 0; i < 16; i++) {
    v[i] = tm.v[i];
  }
  return *this;
}

inline const reTMatrix reTMatrix::operator*(const reTMatrix& tm) const {
  return reTMatrix(*this) *= tm;
}

inline const reVector reTMatrix::mult(const reVector& a, reFloat w) const {
  const reTMatrix& m = *this;
  return reVector(
    a[0]*m[0][0] + a[1]*m[0][1] + a[2]*m[0][2] + w*m[0][3],
    a[0]*m[1][0] + a[1]*m[1][1] + a[2]*m[1][2] + w*m[1][3],
    a[0]*m[2][0] + a[1]*m[2][1] + a[2]*m[2][2] + w*m[2][3]
  );
}

inline reTMatrix& reTMatrix::preMult(const reTMatrix& tm) {
  const reTMatrix tmp(*this);
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

inline const reVector reTMatrix::preMult(const reVector& a, reFloat w) const {
  const reTMatrix& m = *this;
  return reVector(
    a[0]*m[0][0] + a[1]*m[1][0] + a[2]*m[2][0] + w*m[3][0],
    a[0]*m[0][1] + a[1]*m[1][1] + a[2]*m[2][1] + w*m[3][1],
    a[0]*m[0][2] + a[1]*m[1][2] + a[2]*m[2][2] + w*m[3][2]
  );
}

inline reTMatrix& reTMatrix::translate(reFloat x, reFloat y, reFloat z) {
  v[3]  += x;
  v[7]  += y;
  v[11] += z;
  return *this;
}

inline reTMatrix& reTMatrix::scale(reFloat x, reFloat y, reFloat z) {
  for (int i = 0; i < 3; i++) {
    v[4*i + 0] *= x;
    v[4*i + 1] *= y;
    v[4*i + 2] *= z;
  }
  return *this;
}

inline reTMatrix& reTMatrix::rotate(reFloat angle, reFloat x, reFloat y, reFloat z) {
  // refer to Axis-Angle rotation formula
  const float s = reSin(angle);
  const float c = reCos(angle);
  const float c1 = 1 - c;
  const float l = reSqrt(x*x + y*y + z*z);
  x /= l;
  y /= l;
  z /= l;
  const reFloat abc[] = {
      c + c1*x*x, c1*x*y - s*z, c1*x*z + s*y, 0.0f,
    c1*x*y + s*z,   c + c1*y*y, c1*y*z - s*x, 0.0f,
    c1*x*z - s*y, c1*y*z + s*x,   c + c1*z*z, 0.0f,
            0.0f,         0.0f,         0.0f, 1.0f
  };
  this->preMult(reTMatrix(&abc[0]));
  return *this;
}

#endif
