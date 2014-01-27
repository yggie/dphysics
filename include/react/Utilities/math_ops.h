/**
 * @file
 * Contains the definitions of all math operations on custom structs
 */
#ifndef RE_MATH_OPS_H
#define RE_MATH_OPS_H

namespace re {
  inline const reMatrix outer(const reVector& a, const reVector& b) {
    return reMatrix(
      a[0]*b[0], a[0]*b[1], a[0]*b[2],
      a[1]*b[0], a[1]*b[1], a[1]*b[2],
      a[2]*b[0], a[2]*b[1], a[2]*b[2]
    );
  }
  
  inline const reMatrix skew(const reVector& v) {
    return reMatrix(
          0, -v[2],  v[1],
       v[2],     0, -v[0],
      -v[1],  v[0],     0
    );
  }
  
  inline reFloat lengthSq(const reVector& a) {
    return a.x*a.x + a.y*a.y + a.z*a.z;
  }
  
  inline reFloat length(const reVector& a) {
    return reSqrt(lengthSq(a));
  }
  
  inline reVector normalize(const reVector& a) {
    return reVector(a) /= length(a);
  }
  
  inline const reMatrix operator*(reFloat s, const reMatrix& m) {
    return m * s;
  }
  
  inline const reVector operator*(reFloat s, const reVector& v) {
    return v * s;
  }
  
  inline const quat operator*(reFloat s, const quat& q) {
    return q * s;
  }

  inline const quat operator*(const reVector& v, const quat& q) {
    return quat(
      -v.x * q.i - v.y * q.j - v.z * q.k,
      v.x * q.r + v.y * q.k - v.z * q.j,
      -v.x * q.k + v.y * q.r + v.z * q.i,
      v.x * q.j - v.y * q.i + v.z * q.r
    );
  }

  inline const reMatrix axisAngle(const reVector& axis, reFloat angle) {
    const reFloat c = reCos(angle);
    const reFloat s = reSin(angle);
    const reVector a = normalize(axis);
    return reMatrix(c) + (1 - c) * outer(a, a) + skew(a) * s;
  }
}

#endif
