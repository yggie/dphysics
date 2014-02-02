/**
 * @file
 * Contains the definition for the re::vec3 class
 */
#ifndef RE_VECTOR_H
#define RE_VECTOR_H

#include "react/Math/math_common.h"

namespace re {

  /**
   * @ingroup maths
   * Represents a single 3D vector
   */
 
  struct vec3 {
    /** default constructor zeroes all values */
    vec3();
    vec3(reFloat s);
    /** copy constructor @param a The vec3 to copy */
    vec3(const vec3& a);
    vec3(const reFloat* array);
    /** initializes the vector with the input coordinates */
    vec3(reFloat _x, reFloat _y, reFloat _z);
    
    // inline functions
    reFloat& operator[](int i);
    const reFloat& operator[](int i) const;
    const vec3 operator-() const;
    
    // compound assignment with vector input
    vec3& operator=(const vec3& a);
    vec3& operator+=(const vec3& a);
    vec3& operator-=(const vec3& a);
    vec3& operator*=(const vec3& a);
    
    // compound assignment with scalar inputs
    vec3& operator+=(reFloat s);
    vec3& operator-=(reFloat s);
    vec3& operator*=(reFloat s);
    vec3& operator/=(reFloat s);
    
    // arithmetic with vectors
    const vec3 operator+(const vec3& a) const;
    const vec3 operator-(const vec3& a) const;
    const vec3 operator*(const vec3& a) const;
    
    // arithmetic with scalars
    const vec3 operator+(reFloat s) const;
    const vec3 operator-(reFloat s) const;
    const vec3 operator*(reFloat s) const;
    const vec3 operator/(reFloat s) const;
    
    bool equals(const vec3& a) const;
    void set(reFloat _x, reFloat _y, reFloat _z);
    void set(const vec3& a);
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
    
    static const vec3 rand(reFloat b = 1.0);
    static const vec3 unit();
  };
  
  typedef vec3 vec;

  // inline constructors
  inline vec3::vec3() : v{0.0} {
    // do nothing
  }
  
  inline vec3::vec3(reFloat s) : v{s} {
    // do nothing
  }
  
  inline vec3::vec3(const vec3& a) : x(a.x), y(a.y), z(a.z) {
    // do nothing
  }

  /**
   * @brief Copies the first 3 elements of the input array into the vec3
   *
   * @param array An array of values
   */

  inline vec3::vec3(const reFloat* array) : v{0.0} {
    for (int i = 0; i < 3; i++) {
      v[i] = array[i];
    }
  }

  inline vec3::vec3(reFloat _x, reFloat _y, reFloat _z) : x(_x), y(_y), z(_z) { }

  /**
   * @brief Access an element in the vector
   * 
   * @param i The position in the vector to access
   * @return The value of the element in the vector
   */

  inline reFloat& vec3::operator[](int i) {
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

  inline const reFloat& vec3::operator[](int i) const {
    RE_ASSERT(i < 3)
    return v[i];
  }

  /**
   * Returns the vector rotated by 180 degrees about its perpendicular axis
   * 
   * @return The rotated vector
   */

  inline const vec3 vec3::operator-() const {
    return vec3(*this) *= -1.0;
  }

  /**
   * Set the vector to be equal to another vector
   * 
   * @param a The vector operand
   * @return A reference to the resulting vector
   */

  inline vec3& vec3::operator=(const vec3& a) {
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

  inline vec3& vec3::operator+=(const vec3& a) {
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

  inline vec3& vec3::operator-=(const vec3& a) {
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

  inline vec3& vec3::operator*=(const vec3& a) {
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

  inline vec3& vec3::operator+=(reFloat s) {
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

  inline vec3& vec3::operator-=(reFloat s) {
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

  inline vec3& vec3::operator*=(reFloat s) {
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

  inline vec3& vec3::operator/=(reFloat s) {
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

  inline const vec3 vec3::operator+(const vec3& a) const {
    return vec3(*this) += a;
  }

  /**
   * Finds the difference between two vectors
   * 
   * @param a The other vector operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator-(const vec3& a) const {
    return vec3(*this) -= a;
  }

  /**
   * Multiplies two vectors component-wise and returns the result
   * 
   * @param a The other vector operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator*(const vec3& a) const {
    return vec3(*this) *= a;
  }

  /**
   * Add the scalar input to each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator+(reFloat s) const {
    return vec3(*this) += s;
  }

  /**
   * Subtract the scalar input from each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator-(reFloat s) const {
    return vec3(*this) -= s;
  }

  /**
   * Multiplies the scalar input to each element in the vector
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator*(reFloat s) const {
    return vec3(*this) *= s;
  }

  /**
   * Divides each element in the vector by the scalar value
   * 
   * @param s The scalar operand
   * @return The resulting vector
   */

  inline const vec3 vec3::operator/(reFloat s) const {
    return vec3(*this) /= s;
  }

  /**
   * Compares two vectors taking into account reFloating point tolerance
   * 
   * @param a The vector operand
   * @return True if both vectors are similar
   */

  inline bool vec3::equals(const vec3& a) const {
    for (int i = 0; i < 3; i++) {
      if (re::abs(v[i] - a.v[i]) > RE_FP_TOLERANCE) {
        return false;
      }
    }
    return true;
  }

  /**
   * Set the vector to the given values
   * 
   * @param _x The new x-coordinate of the vector
   * @param _y The new y-coordinate of the vector
   * @param _z The new z-coordinate of the vector
   * @param _w The new w-coordinate of the vector
   */

  inline void vec3::set(reFloat _x, reFloat _y, reFloat _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  /**
   * Set the vector to be equal to the input vector
   * 
   * @param a The vector to copy
   */

  inline void vec3::set(const vec3& a) {
    *this = a;
  }

  /**
   * Zero the vector elements
   */

  inline void vec3::setZero() {
    x = y = z = 0.0;
  }
  
  /**
   * Returns a random vector with elements within the range of the given b.
   * i.e. -input < element < input
   * 
   * @param b The upper/lower b of the elements
   * @return The generated vector
   */

  inline const vec3 vec3::rand(reFloat b) {
    return vec3(re::randf(-b, b), re::randf(-b, b), re::randf(-b, b));
  }
}

#include "react/Math/vec3_ops.h"

#endif
