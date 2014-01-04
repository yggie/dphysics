#include "react/reMatrix.h"

const reMatrix reMatrix::IDENTITY(1.0f);

const reMatrix reMatrix::ZERO(0.0f);

/**
 * Multiplies two matrix using matrix multiplication
 * 
 * @param m The matrix operand
 * @return The resulting matrix
 */

const reMatrix reMatrix::operator*(const reMatrix& m) const {
  reMatrix tmp(reMatrix::ZERO);
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        tmp[i][j] += (*this)[i][k] + m[k][i];
      }
    }
  }
  
  return tmp;
}
