/**
 * @file
 * Contains the definition for the re::mat3x3 class
 */
#ifndef RE_MATRIX_H
#define RE_MATRIX_H

#include "react/Math/vec3.h"

namespace re{

  /**
   * @ingroup maths
   * Represents a 3x3 matrix
   */
 
  struct mat3x3 {
    /** Default constructor creates an identity matrix */
    mat3x3();
    /** Copy constructor */
    mat3x3(const mat3x3& m);
    /** Creates a diagonal matrix from the input scalar */
    mat3x3(reFloat s);
    /** Creates a matrix from the given vectors, treating each as a column in the new matrix */
    mat3x3(const re::vec3& a, const re::vec3& b, const re::vec3& c);
    /** Creates a diagonal matrix from the input scalars */
    mat3x3(reFloat m11, reFloat m22, reFloat m33);
    /** Creates the matrix defined element by element from scalar inputs */
    mat3x3(reFloat m11, reFloat m12, reFloat m13,
          reFloat m21, reFloat m22, reFloat m23,
          reFloat m31, reFloat m32, reFloat m33);

    // inline functions
    reFloat* ptr();
    const reFloat* cptr() const;
    
    // accessor methods
    reFloat* operator[](int i);
    const reFloat* operator[](int i) const;
    
    // copy operation
    mat3x3& operator=(const mat3x3& m);
    
    // arithmetic operations
    mat3x3& operator+=(const mat3x3& m);
    mat3x3& operator-=(const mat3x3& m);
    mat3x3& operator*=(const mat3x3& m);
    mat3x3& operator*=(reFloat s);
    mat3x3& operator/=(reFloat s);
    const vec3 operator*(const vec3& v) const;
    const mat3x3 operator*(reFloat s) const;
    const mat3x3 operator/(reFloat s) const;
    
    // arithmetic operations with matrices
    const mat3x3 operator+(const mat3x3& m) const;
    const mat3x3 operator-(const mat3x3& m) const;
    const mat3x3 operator*(const mat3x3& m) const;
    
    /** the elements of the matrix stored as an array */
    reFloat e[9];
    
    static const mat3x3 rand(reFloat b = 1.0);
    static const mat3x3 rotation(reFloat angle, const re::vec3& axis);
    static const mat3x3 rotation(reFloat angle, reFloat x, reFloat y, reFloat z);
  };
  
  typedef mat3x3 mat3;

  inline mat3x3::mat3x3() : mat3x3(1.0) {
    // do nothing
  }

  inline mat3x3::mat3x3(const mat3x3& m) {
    *this = m;
  }

  inline mat3x3::mat3x3(reFloat s) : mat3x3(s, s, s) {
    // do nothing
  }

  inline mat3x3::mat3x3(const re::vec3& a, const re::vec3& b, const re::vec3& c) {
    e[0] = a[0];
    e[1] = a[1];
    e[2] = a[2];

    e[3] = b[0];
    e[4] = b[1];
    e[5] = b[2];

    e[6] = c[0];
    e[7] = c[1];
    e[8] = c[2];
  }

  inline mat3x3::mat3x3(reFloat m11, reFloat m22, reFloat m33) : e{0.0} {
    e[0] = m11;
    e[4] = m22;
    e[8] = m33;
  }

  inline mat3x3::mat3x3(reFloat m11, reFloat m12, reFloat m13,
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

  inline reFloat* mat3x3::ptr() {
    return &e[0];
  }

  /**
   * Returns a const pointer to the raw matrix data
   * 
   * @return A const pointer to the raw data
   */

  inline const reFloat* mat3x3::cptr() const {
    return &e[0];
  }

  /**
   * Access a row in the matrix
   * 
   * @param i The row number
   * @return A pointer to the first element in the row
   */

  inline reFloat* mat3x3::operator[](int i) {
    RE_ASSERT(i < 3)
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

  inline const reFloat* mat3x3::operator[](int i) const {
    RE_ASSERT(i < 3)
    return &e[3*i];
  }

  /**
   * Set the matrix to be equal the input
   * 
   * @param m The matrix to copy
   * @return A reference to the matrix
   */

  inline mat3x3& mat3x3::operator=(const mat3x3& m) {
    memcpy(ptr(), m.cptr(), 9*sizeof(reFloat));
    return *this;
  }

  /**
   * Increments the elements of the matrix with the the input matrix
   * 
   * @param m The input matrix
   * @return A reference to the mat3x3
   */

  inline mat3x3& mat3x3::operator+=(const mat3x3& m) {
    for (reUInt i = 0; i < 9; i++) {
      e[i] += m.e[i];
    }
    return *this;
  }

  /**
   * Decrements the elements of the matrix with the the input matrix
   * 
   * @param m The input matrix
   * @return A reference to the mat3x3
   */

  inline mat3x3& mat3x3::operator-=(const mat3x3& m) {
    for (reUInt i = 0; i < 9; i++) {
      e[i] -= m.e[i];
    }
    return *this;
  }

  /**
   * Multiplies the current matrix with the specified matrix
   * 
   * @param m The matrix operand
   * @return The reference to the matrix
   */

  inline re::mat3x3& mat3x3::operator*=(const re::mat3x3& m) {
    mat3x3 tmp(*this);
    
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        (*this)[i][j] = 0.0;
        for (int k = 0; k < 3; k++) {
          (*this)[i][j] += tmp[i][k] * m[k][j];
        }
      }
    }
    
    return *this;
  }

  /**
   * Multiplies the matrix by the input scalar
   * 
   * @param s The scalar operand
   * @return A reference to the matrix
   */

  inline mat3x3& mat3x3::operator*=(reFloat s) {
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

  inline mat3x3& mat3x3::operator/=(reFloat s) {
    for (int i = 0; i < 9; i++) {
      e[i] /= s;
    }
    return *this;
  }

  /**
   * Multiplies the matrix by a vec3
   * 
   * @param v The vec3 operand
   * @result The resulting vec3
   */

  inline const vec3 mat3x3::operator*(const vec3& v) const {
    return vec3(v[0]*e[0] + v[1]*e[1] + v[2]*e[2],
               v[0]*e[3] + v[1]*e[4] + v[2]*e[5],
               v[0]*e[6] + v[1]*e[7] + v[2]*e[8]);
  }

  /**
   * Multiplies a matrix with a scalar
   * 
   * @param s The scalar operand
   * @return The resulting matrix
   */

  inline const mat3x3 mat3x3::operator*(reFloat s) const {
    return mat3x3(*this) *= s;
  }

  /**
   * Divides a matrix with a scalar
   * 
   * @param s The scalar operand
   * @return The resulting matrix
   */

  inline const mat3x3 mat3x3::operator/(reFloat s) const {
    return mat3x3(*this) /= s;
  }

  /**
   * Adds two matrices
   * 
   * @param m The matrix operand
   * @return The resulting matrix
   */

  inline const mat3x3 mat3x3::operator+(const mat3x3& m) const {
    return mat3x3(*this) += m;
  }

  /**
   * Subtracts two matrices
   * 
   * @param m The matrix operand
   * @return The resulting matrix
   */

  inline const mat3x3 mat3x3::operator-(const mat3x3& m) const {
    return mat3x3(*this) -= m;
  }

  /**
   * Multiplies two matrices together
   * 
   * @param m The matrix operand
   * @return The resulting matrix
   */

  inline const mat3x3 mat3x3::operator*(const mat3x3& m) const {
    return mat3x3(*this) *= m;
  }

  inline const mat3x3 mat3x3::rand(reFloat b) {
    return mat3x3(
      re::randf(-b, b), re::randf(-b, b), re::randf(-b, b),
      re::randf(-b, b), re::randf(-b, b), re::randf(-b, b),
      re::randf(-b, b), re::randf(-b, b), re::randf(-b, b)
    );
  }
}

#include "react/Math/mat3x3_ops.h"

#endif
