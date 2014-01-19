#include "react/Utilities/reQuaternion.h"

const reMatrix reQuaternion::toMatrix() const {
  const reFloat x2 = x*x;
  const reFloat y2 = y*y;
  const reFloat z2 = z*z;
  
  const reFloat wx = w*x;
  const reFloat wy = w*y;
  const reFloat wz = w*z;
  
  const reFloat xy = x*y;
  const reFloat xz = x*z;
  
  const reFloat yz = y*z;
  
  return reMatrix(
    1 - 2*(y2 + z2), 2*(xy + wy), 2*(xz - wy),
    2*(xy - wz), 1 - 2*(x2 + z2), 2*(yz + wx),
    2*(xz + wy), 2*(yz - wx), 1 - 2*(x2 + y2)
  );
}
