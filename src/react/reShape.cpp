#include "react/reShape.h"

#include "react/math.h"

bool reShape::rayIntersect(const reTMatrix& transform, const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) const {
  const reTMatrix inverse = transform.inverse();
  const reVector transfomedOrigin = inverse.mult(origin, 1.0);
  const reVector transfomedDir = inverse.mult(dir, 0.0);
  reVector a, b;
  if (rayIntersect(transfomedOrigin, transfomedDir, &a, &b)) {
    if (intersect != nullptr) {
      *intersect = transform.mult(a, 1.0f);
    }
    if (normal != nullptr) {
      *normal = transform.mult(b, 0.0f).normalized();
    }
    return true;
  } else {
    return false;
  }
  
  return false;
}
