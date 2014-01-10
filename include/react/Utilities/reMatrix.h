/**
 * @file
 * Contains the definition for the reMatrix class
 */
#ifndef RE_MATRIX_H
#define RE_MATRIX_H

#include <string.h>
#include "react/Utilities/reVector.h"

/**
 * Represents a 3x3 matrix
 */

struct reMatrix {
  /** Default constructor creates an identity matrix */
  reMatrix();
  /** Copy constructor */
  reMatrix(const reMatrix& m);
  /** Creates a diagonal matrix from the input scalar */
  reMatrix(reFloat s);
  /** Creates a diagonal matrix from the input scalars */
  reMatrix(reFloat m11, reFloat m22, reFloat m33);
  /** Creates the matrix defined element by element from scalar inputs */
  reMatrix(reFloat m11, reFloat m12, reFloat m13,
        reFloat m21, reFloat m22, reFloat m23,
        reFloat m31, reFloat m32, reFloat m33);

  // inline functions
  reFloat* ptr();
  const reFloat* cptr() const;
  
  // accessor methods
  reFloat* operator[](int i);
  const reFloat* operator[](int i) const;
  
  // copy operation
  reMatrix& operator=(const reMatrix& m);
  
  // arithmetic operations
  reMatrix& operator*=(reFloat s);
  reMatrix& operator/=(reFloat s);
  const reVector operator*(const reVector& v) const;
  const reMatrix operator*(reFloat s) const;
  const reMatrix operator/(reFloat s) const;
  
  // arithmetic operations with matrices
  reMatrix& operator*=(const reMatrix& m);
  const reMatrix operator*(const reMatrix& m) const;
  
  /** the elements of the matrix stored as an array */
  reFloat e[9];
  
  /** defines the identity matrix */
  static const reMatrix IDENTITY;
  /** defines a zero matrix */
  static const reMatrix ZERO;
};

inline reMatrix::reMatrix() { *this = reMatrix::IDENTITY; }
inline reMatrix::reMatrix(const reMatrix& m) { *this = m; }
inline reMatrix::reMatrix(reFloat s) : reMatrix(s, s, s) { }

inline reMatrix::reMatrix(reFloat m11, reFloat m22, reFloat m33) : e{0.0} {
  e[0] = m11;
  e[4] = m22;
  e[8] = m33;
}

inline reMatrix::reMatrix(reFloat m11, reFloat m12, reFloat m13,
                    reFloat m21, reFloat m22, reFloat m23,
                    reFloat m31, reFloat m32, reFloat m33) {
  e[0] = m11;
  e[1] = m12;
  e[2] = m13;
  
  e[3] = m21;
  e[4] = m22;
  e[5] = m23;
  
  e[6] = m31;
  e[7] = m32;
  e[8] = m33;
}

/**
 * Returns a pointer to the raw matrix data
 * 
 * @return A pointer to the raw data
 */

inline reFloat* reMatrix::ptr() {
  return &e[0];
}

/**
 * Returns a const pointer to the raw matrix data
 * 
 * @return A const pointer to the raw data
 */

inline const reFloat* reMatrix::cptr() const {
  return &e[0];
}

/**
 * Access a row in the matrix
 * 
 * @param i The row number
 * @return A pointer to the first element in the row
 */

inline reFloat* reMatrix::operator[](int i) {
  return &e[3*i];
}

/**
 * Access a row in the matrix
 * 
 * ``const`` version
 * 
 * @param i The row number
 * @return A pointer to the first element in the row
 */

inline const reFloat* reMatrix::operator[](int i) const {
  return &e[3*i];
}

/**
 * Set the matrix to be equal the input
 * 
 * @param m The matrix to copy
 * @return A reference to the matrix
 */

inline reMatrix& reMatrix::operator=(const reMatrix& m) {
  memcpy(ptr(), m.cptr(), 9*sizeof(reFloat));
  return *this;
}

/**
 * Multiplies the matrix by the input scalar
 * 
 * @param s The scalar operand
 * @return A reference to the matrix
 */

inline reMatrix& reMatrix::operator*=(reFloat s) {
  for (int i = 0; i < 9; i++) {
    e[i] *= s;
  }
  return *this;
}

/**
 * Divides the matrix by the input scalar
 * 
 * @param s The scalar operand
 * @return A reference to the matrix
 */

inline reMatrix& reMatrix::operator/=(reFloat s) {
  for (int i = 0; i < 9; i++) {
    e[i] /= s;
  }
  return *this;
}

/**
 * Multiplies the matrix by a reVector
 * 
 * @param v The reVector operand
 * @result The resulting reVector
 */

inline const reVector reMatrix::operator*(const reVector& v) const {
  return reVector(v[0]*e[0] + v[1]*e[1] + v[2]*e[2],
             v[0]*e[3] + v[1]*e[4] + v[2]*e[5],
             v[0]*e[6] + v[1]*e[7] + v[2]*e[8]);
}

/**
 * Multiplies a matrix with a scalar
 * 
 * @param s The scalar operand
 * @return The resulting matrix
 */

inline const reMatrix reMatrix::operator*(reFloat s) const {
  return reMatrix(*this) *= s;
}

/**
 * Divides a matrix with a scalar
 * 
 * @param s The scalar operand
 * @return The resulting matrix
 */

inline const reMatrix reMatrix::operator/(reFloat s) const {
  return reMatrix(*this) /= s;
}

/**
 * Multiplies two matrices together
 * 
 * @param m The matrix operand
 * @return The resulting matrix
 */

inline const reMatrix reMatrix::operator*(const reMatrix& m) const {
  return reMatrix(*this) *= m;
}

#endif
