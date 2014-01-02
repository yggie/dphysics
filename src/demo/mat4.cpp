#include "demo/mat4.h"

const mat4 mat4::IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f );

const mat4 mat4::ZERO( 0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f);

inline mat4::mat4(float v11, float v12, float v13, float v14,
                  float v21, float v22, float v23, float v24,
                  float v31, float v32, float v33, float v34,
                  float v41, float v42, float v43, float v44) {
    value[0][0] = v11;
    value[0][1] = v12;
    value[0][2] = v13;
    value[0][3] = v14;
    value[1][0] = v21;
    value[1][1] = v22;
    value[1][2] = v23;
    value[1][3] = v24;
    value[2][0] = v31;
    value[2][1] = v32;
    value[2][2] = v33;
    value[2][3] = v34;
    value[3][0] = v41;
    value[3][1] = v42;
    value[3][2] = v43;
    value[3][3] = v44;
}

bool mat4::operator==(const mat4& m) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (value[i][j] == m.value[i][j]) {
                return false;
            }
        }
    }
    return true;
}

const mat4& mat4::operator*=(const mat4& m) {
    mat4 tmp(*this);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            value[i][j] = tmp.value[i][0] * m.value[0][j] +
                          tmp.value[i][1] * m.value[1][j] +
                          tmp.value[i][2] * m.value[2][j] +
                          tmp.value[i][3] * m.value[3][j];
        }
    }

    return *this;
}

const mat4& mat4::preMult(const mat4& m) {
    mat4 tmp(*this);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            value[j][i] = tmp.value[0][i] * m.value[j][0] +
                          tmp.value[1][i] * m.value[j][1] +
                          tmp.value[2][i] * m.value[j][2] +
                          tmp.value[3][i] * m.value[j][3];
        }
    }

    return *this;
}

void mat4::rotate(float angle, float x, float y, float z) {
    // refer to Axis-Angle rotation formula
    float s = sin(angle);
    float c = cos(angle);
    float c1 = 1 - c;
    float l = sqrt(x*x + y*y + z*z);
    x = x/l;
    y = y/l;
    z = z/l;
    mat4 tmp(    c + c1*x*x, c1*x*y - s*z, c1*x*z + s*y, 0.0f,
               c1*x*y + s*z,   c + c1*y*y, c1*y*z - s*x, 0.0f,
               c1*x*z - s*y, c1*y*z + s*x,   c + c1*z*z, 0.0f,
                       0.0f,         0.0f,         0.0f, 1.0f);
    this->preMult(tmp);
}

void mat4::rotateZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    mat4 tmp(  c, -s,  0,  0,
               s,  c,  0,  0,
               0,  0,  1,  0,
               0,  0,  0,  1  );
    this->preMult(tmp);
}

