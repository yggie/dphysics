#include "react/reShape.h"

#include "react/math.h"
#include "react/reAABB.h"

void reShape::updateAABB(const reMatrix& parentRotation) {
  _aabb.dimens().setZero();
  for (reUInt i = 0; i < numVerts(); i++) {
    const reVector v = parentRotation * vert(i);
    
    for (reUInt j = 0; j < 3; j++) {
      const reFloat d = reAbs(v[j]) + shell();
      if (d > _aabb.dimens()[j]) {
        _aabb.dimens()[j] = d;
      }
    }
  }
}

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
