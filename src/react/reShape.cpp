#include "react/reShape.h"

#include "react/math.h"
#include "react/reAABB.h"

void reShape::updateAABB(const reMatrix& parentRotation) {
  _aabb.set(0, 0, 0);
  for (reUInt i = 0; i < numVerts(); i++) {
    const reVector v = parentRotation * vert(i);
    
    const reFloat nx = reAbs(v.x) + shell();
    if (nx > _aabb.halfWidth()) {
      _aabb.setHalfWidth(nx);
    }
    
    const reFloat ny = reAbs(v.y) + shell();
    if (ny > _aabb.halfHeight()) {
      _aabb.setHalfWidth(ny);
    }
    
    const reFloat nz = reAbs(v.z) + shell();
    if (nz > _aabb.halfDepth()) {
      _aabb.setHalfWidth(nz);
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
