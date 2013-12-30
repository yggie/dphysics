#ifndef RE_MATRIX_H
#define RE_MATRIX_H

#include <string.h>
#include "react/vector.h"

namespace re {
  
  /**
   * Represents a 3x3 matrix
   */
  
  struct mat3f {
    /** Default constructor creates an identity matrix */
    mat3f();
    /** Copy constructor */
    mat3f(const mat3f& m);
    /** Creates a diagonal matrix from the input scalar */
    mat3f(reFloat s);
    /** Creates a diagonal matrix from the input scalars */
    mat3f(reFloat m11, reFloat m22, reFloat m33);
    /** Creates the matrix defined element by element from scalar inputs */
    mat3f(reFloat m11, reFloat m12, reFloat m13,
          reFloat m21, reFloat m22, reFloat m23,
          reFloat m31, reFloat m32, reFloat m33);

    // inline functions
    float* ptr();
    const float* cptr() const;
    
    // accessor methods
    float* operator[](int i);
    const float* operator[](int i) const;
    
    // copy operation
    mat3f& operator=(const mat3f& m);
    
    // arithmetic operations (inlined)
    mat3f& operator*=(reFloat s);
    const vec operator*(const vec& v) const;
    const mat3f operator*(reFloat s) const;
    
    // arithmetic operations (not inlined)
    const mat3f operator*(const mat3f& m) const;
    
    /** the elements of the matrix stored as an array */
    float e[9];
    
    /** defines the identity matrix */
    static const mat3f IDENTITY;
    /** defines a zero matrix */
    static const mat3f ZERO;
  };
  
  typedef mat3f mat;
  
  inline mat3f::mat3f() { *this = mat3f::IDENTITY; }
  inline mat3f::mat3f(const mat3f& m) { *this = m; }
  inline mat3f::mat3f(reFloat s) : mat3f(s, s, s) { }
  
  inline mat3f::mat3f(reFloat m11, reFloat m22, reFloat m33) : e{0.0} {
    e[0] = m11;
    e[4] = m22;
    e[8] = m33;
  }
  
  inline mat3f::mat3f(reFloat m11, reFloat m12, reFloat m13,
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
  
  inline float* mat3f::ptr() {
    return &e[0];
  }
  
  /**
   * Returns a const pointer to the raw matrix data
   * 
   * @return A const pointer to the raw data
   */
  
  inline const float* mat3f::cptr() const {
    return &e[0];
  }
  
  /**
   * Access a row in the matrix
   * 
   * @param i The row number
   * @return A pointer to the first element in the row
   */
  
  inline float* mat3f::operator[](int i) {
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
  
  inline const float* mat3f::operator[](int i) const {
    return &e[3*i];
  }
  
  /**
   * Set the matrix to be equal the input
   * 
   * @param m The matrix to copy
   * @return A reference to the matrix
   */
  
  inline mat3f& mat3f::operator=(const mat3f& m) {
    memcpy(ptr(), m.cptr(), 9*sizeof(float));
    return *this;
  }
  
  /**
   * Multiplies the matrix by the input scalar
   * 
   * @param s The scalar operand
   * @return A reference to the matrix
   */
  
  inline mat3f& mat3f::operator*=(reFloat s) {
    for (int i = 0; i < 9; i++) {
      e[i] *= s;
    }
    return *this;
  }
  
  /**
   * Multiplies the matrix by a vector
   * 
   * @param v The vector operand
   * @result The resulting vector
   */
  
  inline const vec mat3f::operator*(const vec& v) const {
    return vec(v[0]*e[0] + v[1]*e[1] + v[2]*e[2],
               v[0]*e[3] + v[1]*e[4] + v[2]*e[5],
               v[0]*e[6] + v[1]*e[7] + v[2]*e[8]);
  }
  
  /**
   * Multiplies a matrix with a scalar
   * 
   * @param s The scalar operand
   * @return The resulting matrix
   */
  
  inline const mat3f mat3f::operator*(reFloat s) const {
    return mat3f(*this) *= s;
  }
}

#endif
