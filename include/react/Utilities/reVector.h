/**
 * @file
 * Contains the definition for the reVector class
 */
#ifndef RE_VECTOR_H
#define RE_VECTOR_H

#include "react/Utilities/math_funcs.h"

/**
 * @brief Represents a single 3D vector
 */

struct reVector {
  /** default constructor zeroes all values */
  reVector();
  /** copy constructor @param a The reVector to copy */
  reVector(const reVector& a);
  reVector(const reFloat* array);
  /** initializes the vector with the input coordinates */
  reVector(reFloat _x, reFloat _y, reFloat _z);
  
  // inline functions
  reFloat& operator[](int i);
  const reFloat& operator[](int i) const;
  const reVector operator-() const;
  
  // compound assignment with vector input
  reVector& operator=(const reVector& a);
  reVector& operator+=(const reVector& a);
  reVector& operator-=(const reVector& a);
  reVector& operator*=(const reVector& a);
  
  // compound assignment with scalar inputs
  reVector& operator+=(reFloat s);
  reVector& operator-=(reFloat s);
  reVector& operator*=(reFloat s);
  reVector& operator/=(reFloat s);
  
  // arithmetic with vectors
  const reVector operator+(const reVector& a) const;
  const reVector operator-(const reVector& a) const;
  const reVector operator*(const reVector& a) const;
  
  // arithmetic with scalars
  const reVector operator+(reFloat s) const;
  const reVector operator-(reFloat s) const;
  const reVector operator*(reFloat s) const;
  const reVector operator/(reFloat s) const;
  
  // vector operations
  reFloat dot(const reVector& a) const;
  const reVector cross(const reVector& a) const;
  reFloat length() const;
  reFloat lengthSq() const;
  
  bool equals(const reVector& a) const;
  void normalize();
  reVector& normalized();
  const reVector normalized() const;
  void set(reFloat _x, reFloat _y, reFloat _z);
  void set(const reVector& a);
  void setZero();
  
  /** allows the vector to be accessed using common element names (x, y, z) or as elements of an array */
  union {
    struct {
      /** x-coordinate of the vector */
      reFloat x;
      /** y-coordinate of the vector */
      reFloat y;
      /** z-coordinate of the vector */
      reFloat z;
    };
    struct {
      /** the vector as an array */
      reFloat v[3];
    };
  };
  
  static const reVector random();
};

// inline constructors
inline reVector::reVector() : v{0.0} { }
inline reVector::reVector(const reVector& a) : x(a.x), y(a.y), z(a.z) { }

/**
 * @brief Copies the first 3 elements of the input array into the reVector
 *
 * @param array An array of values
 */

inline reVector::reVector(const reFloat* array) : v{0.0} {
  for (int i = 0; i < 3; i++) {
    v[i] = array[i];
  }
}

inline reVector::reVector(reFloat _x, reFloat _y, reFloat _z) : x(_x), y(_y), z(_z) { }

/**
 * @brief Access an element in the vector
 * 
 * @param i The position in the vector to access
 * @return The value of the element in the vector
 */

inline reFloat& reVector::operator[](int i) {
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

inline const reFloat& reVector::operator[](int i) const {
  return v[i];
}

/**
 * Returns the vector rotated by 180 degrees about its perpendicular axis
 * 
 * @return The rotated vector
 */

inline const reVector reVector::operator-() const {
  return reVector(*this) *= -1.0;
}

/**
 * Set the vector to be equal to another vector
 * 
 * @param a The vector operand
 * @return A reference to the resulting vector
 */

inline reVector& reVector::operator=(const reVector& a) {
  for (int i = 0; i < 3; i++) {
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

inline reVector& reVector::operator+=(const reVector& a) {
  for (int i = 0; i < 3; i++) {
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

inline reVector& reVector::operator-=(const reVector& a) {
  for (int i = 0; i < 3; i++) {
    v[i] -= a.v[i];
  }
  return *this;
}

/**
 * Multiplies two vectors component-wise
 * 
 * @param a The vector operand
 * @return A reference to the resulting vector
 */

inline reVector& reVector::operator*=(const reVector& a) {
  for (int i = 0; i < 3; i++) {
    v[i] *= a.v[i];
  }
  return *this;
}

/**
 * Increments the vector by a scalar value
 * 
 * @param s The scalar operand
 * @return A reference to the resulting vector
 */

inline reVector& reVector::operator+=(reFloat s) {
  for (int i = 0; i < 3; i++) {
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

inline reVector& reVector::operator-=(reFloat s) {
  for (int i = 0; i < 3; i++) {
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

inline reVector& reVector::operator*=(reFloat s) {
  for (int i = 0; i < 3; i++) {
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

inline reVector& reVector::operator/=(reFloat s) {
  for (int i = 0; i < 3; i++) {
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

inline const reVector reVector::operator+(const reVector& a) const {
  return reVector(*this) += a;
}

/**
 * Finds the difference between two vectors
 * 
 * @param a The other vector operand
 * @return The resulting vector
 */

inline const reVector reVector::operator-(const reVector& a) const {
  return reVector(*this) -= a;
}

/**
 * Multiplies two vectors component-wise and returns the result
 * 
 * @param a The other vector operand
 * @return The resulting vector
 */

inline const reVector reVector::operator*(const reVector& a) const {
  return reVector(*this) *= a;
}

/**
 * Add the scalar input to each element in the vector
 * 
 * @param s The scalar operand
 * @return The resulting vector
 */

inline const reVector reVector::operator+(reFloat s) const {
  return reVector(*this) += s;
}

/**
 * Subtract the scalar input from each element in the vector
 * 
 * @param s The scalar operand
 * @return The resulting vector
 */

inline const reVector reVector::operator-(reFloat s) const {
  return reVector(*this) -= s;
}

/**
 * Multiplies the scalar input to each element in the vector
 * 
 * @param s The scalar operand
 * @return The resulting vector
 */

inline const reVector reVector::operator*(reFloat s) const {
  return reVector(*this) *= s;
}

/**
 * Divides each element in the vector by the scalar value
 * 
 * @param s The scalar operand
 * @return The resulting vector
 */

inline const reVector reVector::operator/(reFloat s) const {
  return reVector(*this) /= s;
}

/**
 * Calculates the vector dot product between two vectors
 * 
 * @param a The vector operand
 * @return The resulting dot product
 */

inline reFloat reVector::dot(const reVector& a) const {
  return x*a.x + y*a.y + z*a.z;
}

/**
 * Calculates the cross product between two vectors. Ignores the 4th element
 * 
 * @param a The vector operand
 * @return The resulting vector
 */

inline const reVector reVector::cross(const reVector& a) const {
  return reVector(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
}

/**
 * Calculates the length of the vector
 * 
 * @return The length of the vector
 */

inline reFloat reVector::length() const {
  return reSqrt(lengthSq());
}

/**
 * Calculates the squared of the length of the vector
 * 
 * @return The vector's length squared
 */

inline reFloat reVector::lengthSq() const {
  return x*x + y*y + z*z;
}

/**
 * Compares two vectors taking into account reFloating point tolerance
 * 
 * @param a The vector operand
 * @return True if both vectors are similar
 */

inline bool reVector::equals(const reVector& a) const {
  for (int i = 0; i < 3; i++) {
    if (reAbs(v[i] - a.v[i]) > RE_FP_TOLERANCE) {
      return false;
    }
  }
  return true;
}

/**
 * Normalizes the vector
 */

inline void reVector::normalize() {
  *this /= length();
}

/**
 * Normalizes the vector, this method can be chained
 * 
 * @return A reference to the reVector
 */

inline reVector& reVector::normalized() {
  normalize();
  return *this;
}

/**
 * Normalizes the vector, this method can be chained
 * 
 * Enforces const constraint by creating a copy
 * 
 * @return A reference to the reVector
 */

inline const reVector reVector::normalized() const {
  return reVector(*this).normalized();
}

/**
 * Set the vector to the given values
 * 
 * @param _x The new x-coordinate of the vector
 * @param _y The new y-coordinate of the vector
 * @param _z The new z-coordinate of the vector
 * @param _w The new w-coordinate of the vector
 */

inline void reVector::set(reFloat _x, reFloat _y, reFloat _z) {
  x = _x;
  y = _y;
  z = _z;
}

/**
 * Set the vector to be equal to the input vector
 * 
 * @param a The vector to copy
 */

inline void reVector::set(const reVector& a) {
  *this = a;
}

/**
 * Zero the vector elements
 */

inline void reVector::setZero() {
  x = y = z = 0.0;
}

inline const reVector reVector::random() {
  return reVector(reRandom() - 0.5, reRandom() - 0.5, reRandom() - 0.5).normalized();
}

#endif
