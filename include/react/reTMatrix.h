/**
 * @file
 * Contains definition of the reTMatrix class
 */
#ifndef RE_TMATRIX_H
#define RE_TMATRIX_H

#include "react/common.h"

/**
 * Defines a transformation matrix in sparse matrix form.
 */

class reTMatrix {
public:
  /** Default constructor initializes the matrix to identity */
  reTMatrix();
  reTMatrix(const reTMatrix& tm);
  reTMatrix(reFloat d);
  /** Destructor does nothing */
  ~reTMatrix();
  
  reFloat* ptr();
  
  reFloat* operator[](int i);
  
  reTMatrix& operator=(const reTMatrix& tm);
  
private:
  reFloat _v[12];
};

inline reTMatrix::reTMatrix() : reTMatrix(1.0) {
  // do nothing
}

inline reTMatrix::reTMatrix(const reTMatrix& tm) {
  for (int i = 0; i < 12; i++) {
    _v[i] = tm._v[i];
  }
}

inline reTMatrix::reTMatrix(reFloat d) : _v{0} {
  _v[0] = d;
  _v[5] = d;
  _v[10] = d;
}

inline reTMatrix::~reTMatrix() {
  // do nothing
}

inline reFloat* reTMatrix::ptr() {
  return &_v[0];
}

inline reFloat* reTMatrix::operator[](int i) {
  return &_v[4*i];
}

inline reTMatrix& reTMatrix::operator=(const reTMatrix& tm) {
  for (int i = 0; i < 12; i++) {
    _v[i] = tm._v[i];
  }
  return *this;
}

#endif
