#include "matrix.h"

using namespace re;

const mat3f mat3f::IDENTITY(1.0f);

const mat3f mat3f::ZERO(0.0f);

/**
 * Multiplies two matrix using matrix multiplication
 * 
 * @param m The matrix operand
 * @return The resulting matrix
 */

const mat3f mat3f::operator*(const mat3f& m) const {
  mat3f tmp(mat3f::ZERO);
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        tmp[i][j] += (*this)[i][k] + m[k][i];
      }
    }
  }
  
  return tmp;
}

