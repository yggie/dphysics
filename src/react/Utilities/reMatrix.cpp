#include "react/Utilities/reMatrix.h"

const reMatrix reMatrix::IDENTITY(1.0f);

const reMatrix reMatrix::ZERO(0.0f);

/**
 * Multiplies the current matrix with the specified matrix
 * 
 * @param m The matrix operand
 * @return The reference to the matrix
 */

reMatrix& reMatrix::operator*=(const reMatrix& m) {
  reMatrix tmp(*this);
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      (*this)[i][j] = 0.0;
      for (int k = 0; k < 3; k++) {
        (*this)[i][j] += tmp[i][k] * m[k][j];
      }
    }
  }
  
  return *this;
}

