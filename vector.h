#ifndef D_VECTOR_H
#define D_VECTOR_H

#include "math.h"

namespace d {
  
  /**
   * Represents a single vector in homogenous coordinates
   */
  
  struct vec4f {
    /** default constructor zeroes all values */
    vec4f();
    /** copy constructor */
    vec4f(const vec4f& a);
    /** initializes the vector with the input coordinates */
    vec4f(dfloat _x, dfloat _y, dfloat _z, dfloat _w = 0.0);
    
    // inline functions
    float& operator[](int i);
    const float& operator[](int i) const;
    float& operator()(int i);
    const float& operator()(int i) const;
    const vec4f operator-() const;
    
    // compound assignment with vector input
    vec4f& operator=(const vec4f& a);
    vec4f& operator+=(const vec4f& a);
    vec4f& operator-=(const vec4f& a);
    
    // compound assignment with scalar inputs
    vec4f& operator+=(dfloat s);
    vec4f& operator-=(dfloat s);
    vec4f& operator*=(dfloat s);
    vec4f& operator/=(dfloat s);
    
    // arithmetic with vectors
    const vec4f operator+(const vec4f& a) const;
    const vec4f operator-(const vec4f& a) const;
    
    // arithmetic with scalars
    const vec4f operator+(dfloat s) const;
    const vec4f operator-(dfloat s) const;
    const vec4f operator*(dfloat s) const;
    const vec4f operator/(dfloat s) const;
    
    // vector operations
    dfloat dot(const vec4f& a) const;
    const vec4f cross(const vec4f& a) const;
    dfloat length() const;
    dfloat lengthSq() const;
    
    bool equals(const vec4f& a) const;
    void normalize();
    void set(dfloat _x, dfloat _y, dfloat _z, dfloat _w = 0.0);
    void set(const vec4f& a);
    void setZero();
    
    /** allows the vector to be accessed using common element names (x, y, z) or as elements of an array */
    union {
      struct {
        /** x-coordinate of the vector */
        float x;
        /** y-coordinate of the vector */
        float y;
        /** z-coordinate of the vector */
        float z;
        /** w-coordinate of the vector */
        float w;
      };
      struct {
        /** the vector as an array */
        float v[4];
      };
    };
  };
  
  typedef vec4f vec;
  
  // inline constructors
  inline vec4f::vec4f() : v{0.0} { }
  inline vec4f::vec4f(const vec4f& a) : x(a.x), y(a.y), z(a.z), w(0.0) { }
  inline vec4f::vec4f(dfloat _x, dfloat _y, dfloat _z, dfloat _w) : x(_x), y(_y), z(_z), w(_w) { }
  
  /**
   * Access an element in the vector
   * 
   * @param i The position in the vector to access
   * @return The value of the element in the vector
   */
  
  inline float& vec4f::operator[](int i) {
    return v[i];
  }
  
  /**
   * Access an element in the vector
   * 
   * ``const`` version
   * 
   * @param i The position in the vector to access
   * @return The value of the element in the vector
   */
  
  inline const float& vec4f::operator[](int i) const {
    return v[i];
  }
  
  /**
   * Access an element in the vector
   * 
   * Does not use zero-based indexing
   * 
   * @param i The position in the vector to access
   * @return The value of the element in the vector
   */
  
  inline float& vec4f::operator()(int i) {
    return v[i + 1];
  }
  
  /**
   * Access an element in the vector
   * 
   * Does not use zero-based indexing. ``const`` version
   * 
   * @param i The position in the vector to access
   * @return The value of the element in the vector
   */
  
  inline const float& vec4f::operator()(int i) const {
    return v[i + 1];
  }
  
  /**
   * Returns the vector rotated by 180 degrees about its perpendicular axis
   * 
   * @return The rotated vector
   */
  
  inline const vec4f vec4f::operator-() const {
    return vec4f(*this) *= -1.0;
  }
  
  /**
   * Set the vector to be equal to another vector
   * 
   * @param a The vector operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator=(const vec4f& a) {
    for (int i = 0; i < 4; i++) {
      v[i] = a.v[i];
    }
    return *this;
  }
  
  /**
   * Increments the vector by another vector
   * 
   * @param a The vector operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator+=(const vec4f& a) {
    for (int i = 0; i < 4; i++) {
      v[i] += a.v[i];
    }
    return *this;
  }
  
  /**
   * Decrements the vector by another vector
   * 
   * @param a The vector operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator-=(const vec4f& a) {
    for (int i = 0; i < 4; i++) {
      v[i] -= a.v[i];
    }
    return *this;
  }
  
  /**
   * Increments the vector by a scalar value
   * 
   * @param s The scalar operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator+=(dfloat s) {
    for (int i = 0; i < 4; i++) {
      v[i] += s;
    }
    return *this;
  }
  
  /**
   * Decrements the vector by a scalar value
   * 
   * @param s The scalar operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator-=(dfloat s) {
    for (int i = 0; i < 4; i++) {
      v[i] -= s;
    }
    return *this;
  }
  
  /**
   * Multiplies the vector by a scalar value
   * 
   * @param s The scalar operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator*=(dfloat s) {
    for (int i = 0; i < 4; i++) {
      v[i] *= s;
    }
    return *this;
  }
  
  /**
   * Divides the vector by a scalar value
   * 
   * @param s The scalar operand
   * @return A reference to the resulting vector
   */
  
  inline vec4f& vec4f::operator/=(dfloat s) {
    for (int i = 0; i < 4; i++) {
      v[i] /= s;
    }
    return *this;
  }
  
  /**
   * Adds two vectors together
   * 
   * @param a The other vector operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator+(const vec4f& a) const {
    return vec4f(*this) += a;
  }
  
  /**
   * Finds the difference between two vectors
   * 
   * @param a The other vector operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator-(const vec4f& a) const {
    return vec4f(*this) -= a;
  }
  
  /**
   * Add the scalar input to each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator+(dfloat s) const {
    return vec4f(*this) += s;
  }
  
  /**
   * Subtract the scalar input from each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator-(dfloat s) const {
    return vec4f(*this) -= s;
  }
  
  /**
   * Multiplies the scalar input to each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator*(dfloat s) const {
    return vec4f(*this) *= s;
  }
  
  /**
   * Divides each element in the vector by the scalar value
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::operator/(dfloat s) const {
    return vec4f(*this) /= s;
  }
  
  /**
   * Calculates the vector dot product between two vectors
   * 
   * @param a The vector operand
   * @return The resulting dot product
   */
  
  inline dfloat vec4f::dot(const vec4f& a) const {
    return x*a.x + y*a.y + z*a.z + w*a.w;
  }
  
  /**
   * Calculates the cross product between two vectors. Ignores the 4th element
   * 
   * @param a The vector operand
   * @return The resulting vector
   */
  
  inline const vec4f vec4f::cross(const vec4f& a) const {
    return vec4f(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.y);
  }
  
  /**
   * Calculates the length of the vector
   * 
   * @return The length of the vector
   */
  
  inline dfloat vec4f::length() const {
    return sqrt(lengthSq());
  }
  
  /**
   * Calculates the squared of the length of the vector
   * 
   * @return The vector's length squared
   */
  
  inline dfloat vec4f::lengthSq() const {
    return x*x + y*y + z*z + w*w;
  }
  
  /**
   * Compares two vectors taking into account floating point tolerance
   * 
   * @param a The vector operand
   * @return True if both vectors are similar
   */
  
  inline bool vec4f::equals(const vec4f& a) const {
    for (int i = 0; i < 4; i++) {
      if (fabs(v[i] - a.v[i]) > D_FP_TOLERANCE) {
        return false;
      }
    }
    return true;
  }
  
  /**
   * Normalizes the vector
   */
  
  inline void vec4f::normalize() {
    *this /= length();
  }
  
  /**
   * Set the vector to the given values
   * 
   * @param _x The new x-coordinate of the vector
   * @param _y The new y-coordinate of the vector
   * @param _z The new z-coordinate of the vector
   * @param _w The new w-coordinate of the vector
   */
  
  inline void vec4f::set(dfloat _x, dfloat _y, dfloat _z, dfloat _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }
  
  /**
   * Set the vector to be equal to the input vector
   * 
   * @param a The vector to copy
   */
  
  inline void vec4f::set(const vec4f& a) {
    *this = a;
  }
  
  /**
   * Zero the vector elements
   */
  
  inline void vec4f::setZero() {
    x = y = z = w = 0.0;
  }
}

#endif
