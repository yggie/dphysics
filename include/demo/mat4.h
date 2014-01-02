#ifndef RE_MAT4_H
#define RE_MAT4_H

#include <string.h>
#include <cmath>

struct mat4 {
    mat4();                                         // inline
    mat4(float v11, float v22, float v33, float v44);   // inline
    mat4(const mat4& m);                            // inline
    mat4(float v11, float v12, float v13, float v14,
         float v21, float v22, float v23, float v24,
         float v31, float v32, float v33, float v34,
         float v41, float v42, float v43, float v44);   // inline

    float* ptr();                                   // inline
    const float* cptr() const;                      // inline

    bool operator==(const mat4& m) const;           // inline
    mat4& operator=(const mat4& m);                 // inline
    const mat4& operator*=(const mat4& m);          // inline
    const mat4& preMult(const mat4& m);             // inline

    const mat4 operator*(const mat4& m) const;      // inline

    void scale(float s);                            // inline
    void scale(float x, float y, float z = 1.0f);   // inline
    void translate(float x, float y, float z = 0.0f);   // inline
    void rotate(float angle, float x, float y, float z);
    void rotateZ(float angle);

    void setZero();                                 // inline
    void setIdentity();                             // inline

    float value[4][4];
    
    inline void frustum(float left, float right, float btm, float top, float near, float far);
    inline void orthogonal(float left, float right, float btm, float top, float near, float far);

    static const mat4 IDENTITY;
    static const mat4 ZERO;
};

inline mat4::mat4() {
    (*this) = mat4::IDENTITY;
}

inline mat4::mat4(float v11, float v22, float v33, float v44) {
    memset(&value[0][0], 0, 16*sizeof(float));
    value[0][0] = v11;
    value[1][1] = v22;
    value[2][2] = v33;
    value[3][3] = v44;
}

inline float* mat4::ptr() {
    return &value[0][0];
}

inline const float* mat4::cptr() const {
    return &value[0][0];
}

inline mat4::mat4(const mat4& m) {
    (*this) = m;
}

inline mat4& mat4::operator=(const mat4& m) {
    memcpy(&value[0][0], &m.value[0][0], 16*sizeof(float));
    return *this;
}

inline const mat4 mat4::operator*(const mat4& m) const {
    return mat4(*this) *= m;
}

inline void mat4::scale(float s) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            value[i][j] *= s;
        }
    }
}

inline void mat4::scale(float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        value[0][i] *= x;
        value[1][i] *= y;
        value[2][i] *= z;
    }
}

inline void mat4::translate(float x, float y, float z) {
    value[0][3] += x*value[3][3];
    value[1][3] += y*value[3][3];
    value[2][3] += z*value[3][3];
}

inline void mat4::frustum(float left, float right, float btm, float top, float near, float far) {
  const float A = 2 * near / (right - left);
  const float B = (right + left) / (right - left);
  const float C = 2 * near / (top - btm);
  const float D = (top + btm) / (top - btm);
  const float E = -(far + near) / (far - near);
  const float F = -2 * far * near / (far - near);
  
  *this *= mat4(
    A,  0,  B,  0,
    0,  C,  D,  0,
    0,  0,  E,  F,
    0,  0, -1,  0
  );
}

inline void mat4::orthogonal(float left, float right, float btm, float top, float back, float front) {
  const float xRng = right - left;
  const float yRng = top - btm;
  const float zRng = front - back;
  
  const float dx = (right + left) / xRng;
  const float dy = (top + btm) / yRng;
  const float dz = (front + back) / zRng;
  
  *this *= mat4(
    2/xRng,      0,      0,     dx,
         0, 2/yRng,      0,     dy,
         0,      0, 2/zRng,     dz,
         0,      0,      0,      1
  );
}

inline void mat4::setZero() {
    memset(&value[0][0], 0, 16*sizeof(float));
}

inline void mat4::setIdentity() {
    (*this) = mat4::IDENTITY;
}

#endif // RE_MAT4_H

