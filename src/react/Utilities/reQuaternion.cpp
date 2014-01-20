#include "react/Utilities/reQuaternion.h"

const reMatrix reQuaternion::toMatrix() const {
  const reFloat i2 = i*i;
  const reFloat j2 = j*j;
  const reFloat k2 = k*k;
  
  const reFloat ri = r*i;
  const reFloat rj = r*j;
  const reFloat rk = r*k;
  
  const reFloat ij = i*j;
  const reFloat ik = i*k;
  
  const reFloat jk = j*k;
  
  return reMatrix(
    1 - 2*(j2 + k2), 2*(ij + rj), 2*(ik - rj),
    2*(ij - rk), 1 - 2*(i2 + k2), 2*(jk + ri),
    2*(ik + rj), 2*(jk - ri), 1 - 2*(i2 + j2)
  );
}
