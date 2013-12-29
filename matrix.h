#ifndef D_MATRIX_H
#define D_MATRIX_H

#include <string.h>
#include "vector.h"

namespace d {
  
  /**
   * Represents a 4x4 matrix
   */
  
  struct mat4f {
    /** Default constructor creates an identity matrix */
    mat4f();
    /** Copy constructor */
    mat4f(const mat4f& m);
    /** Creates a diagonal matrix from the input scalars */
    mat4f(dfloat m11, dfloat m22, dfloat m33, dfloat m44);
    /** Creates the matrix defined element by element from scalar inputs */
    mat4f(dfloat m11, dfloat m12, dfloat m13, dfloat m14,
          dfloat m21, dfloat m22, dfloat m23, dfloat m24,
          dfloat m31, dfloat m32, dfloat m33, dfloat m34,
          dfloat m41, dfloat m42, dfloat m43, dfloat m44);

    float* ptr();
    const float* cptr() const;
    
    mat4f& operator=(const mat4f& m);
    
    float* operator[](int i);
    const float* operator[](int i) const;
    float& operator()(int i, int j);
    const float& operator()(int i, int j) const;
    
    /** the elements of the matrix stored as an array */
    float e[16];
    
    static const mat4f IDENTITY;
  };
  
  typedef mat4f mat4;
  
  inline mat4f::mat4f() { *this = mat4f::IDENTITY; }
  inline mat4f::mat4f(const mat4f& m) { *this = m; }
  
  inline mat4f::mat4f(dfloat m11, dfloat m22, dfloat m33, dfloat m44) : e{0.0} {
    e[0] = m11;
    e[5] = m22;
    e[10] = m33;
    e[15] = m44;
  }
  
  inline mat4f::mat4f(dfloat m11, dfloat m12, dfloat m13, dfloat m14,
                      dfloat m21, dfloat m22, dfloat m23, dfloat m24,
                      dfloat m31, dfloat m32, dfloat m33, dfloat m34,
                      dfloat m41, dfloat m42, dfloat m43, dfloat m44) {
    e[0] = m11;
    e[1] = m12;
    e[2] = m13;
    e[3] = m14;
    
    e[4] = m21;
    e[5] = m22;
    e[6] = m23;
    e[7] = m24;
    
    e[8] = m31;
    e[9] = m32;
    e[10] = m33;
    e[11] = m34;
    
    e[12] = m41;
    e[13] = m42;
    e[14] = m43;
    e[15] = m44;
  }
  
  /**
   * Returns a pointer to the raw matrix data
   * 
   * @return A pointer to the raw data
   */
  
  inline float* mat4f::ptr() {
    return &e[0];
  }
  
  /**
   * Returns a const pointer to the raw matrix data
   * 
   * @return A const pointer to the raw data
   */
  
  inline const float* mat4f::cptr() const {
    return &e[0];
  }
  
  /**
   * Set the matrix to be equal the input
   * 
   * @param m The matrix to copy
   * @return A reference to the matrix
   */
  
  inline mat4f& mat4f::operator=(const mat4f& m) {
    memcpy(ptr(), m.cptr(), 16*sizeof(float));
    return *this;
  }
  
  /**
   * Access a row in the matrix
   * 
   * @param i The row number
   * @return A pointer to the first element in the row
   */
  
  inline float* mat4f::operator[](int i) {
    return &e[4*i];
  }
  
  /**
   * Access a row in the matrix
   * 
   * ``const`` version
   * 
   * @param i The row number
   * @return A pointer to the first element in the row
   */
  
  inline const float* mat4f::operator[](int i) const {
    return &e[4*i];
  }
  
  /**
   * Access elements in the array using row-major ordering.
   * 
   * Does not use zero-based indexing
   * 
   * @param i The row number
   * @param j The column number
   */
  
  inline float& mat4f::operator()(int i, int j) {
    return e[4*(i - 1) + j - 1];
  }
  
  /**
   * Access elements in the array using row-major ordering.
   * 
   * Does not use zero-based indexing. ``const`` version
   * 
   * @param i The row number
   * @param j The column number
   */
  
  inline const float& mat4f::operator()(int i, int j) const {
    return e[4*(i - 1) + j - 1];
  }
}

#endif
