#include "matrix.h"

using namespace d;

const mat4f mat4f::IDENTITY(1.0f);

const mat4f mat4f::ZERO(0.0f);

/**
 * Multiplies two matrix using matrix multiplication
 * 
 * @param m The matrix operand
 * @return The resulting matrix
 */

const mat4f mat4f::operator*(const mat4f& m) const {
  mat4f tmp(mat4f::ZERO);
  
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        tmp[i][j] += (*this)[i][k] + m[k][i];
      }
    }
  }
  
  return tmp;
}

