/**
 * @file
 * Contains the definition of the reDistortedShape class
 */
#ifndef RE_DISTORTEDSHAPE_H
#define RE_DISTORTEDSHAPE_H

#include "react/Collision/Shapes/reShape.h"

/**
 * A special wrapper class around an arbitrary shape which allows the
 * user to implement distortion on the shape based on arbitrary
 * transformations
 */

class reDistortedShape : public reShape {
public:
  reDistortedShape();
  reDistortedShape(const reDistortedShape& other);
  reDistortedShape(const reShape& base);
  ~reDistortedShape();
  
  void setBaseShape(const reShape& base);
  reDistortedShape& withBaseShape(const reShape& base);
  
  void setDistortion(const reTMatrix& distortion);
  reDistortedShape& withDistortion(const reTMatrix& distortion);
  
  // shape representation
  reShape::Type type() const override;
  reUInt numVerts() const override;
  const reVector vert(reUInt i) const override;
  reFloat shell() const override;
  const reVector offset() const override;
  
  // physical metrics
  reFloat volume() const override;
  const reMatrix computeInertia() const override;
  
  bool intersectsRay(const reVector& origin, const reVector& dir, reVector* intersect = nullptr, reVector* normal = nullptr) const override;
  
  bool intersectsRay(const reTMatrix& transform, const reVector& origin, const reVector& dir, reVector* intersect = nullptr, reVector* normal = nullptr) const override;
  
private:
  reTMatrix _distortion;
  reShape* _shape;
};

inline reDistortedShape::reDistortedShape() : _distortion(),
_shape(nullptr) {
  // do nothing
}

inline void reDistortedShape::setDistortion(const reTMatrix& distortion) {
  _distortion = distortion;
}

inline reDistortedShape& reDistortedShape::withDistortion(const reTMatrix& distortion) {
  setDistortion(distortion);
  return *this;
}

inline reShape::Type reDistortedShape::type() const {
  return reShape::DISTORTED;
}

inline reUInt reDistortedShape::numVerts() const {
  if (_shape != nullptr) {
    return _shape->numVerts();
  } else {
    return 0;
  }
}

inline const reVector reDistortedShape::vert(reUInt i) const {
  if (_shape != nullptr) {
    return _shape->vert(i);
  } else {
    return reVector(0.0, 0.0, 0.0);
  }
}

inline reFloat reDistortedShape::shell() const {
  if (_shape != nullptr) {
    return _shape->shell();
  } else {
    return reShape::shell();
  }
}

inline const reVector reDistortedShape::offset() const {
  if (_shape != nullptr) {
    return _shape->offset() + reVector(_distortion[0][3], _distortion[1][3], _distortion[2][3]);
  } else {
    return reVector(_distortion[0][3], _distortion[1][3], _distortion[2][3]);
  }
}

inline reFloat reDistortedShape::volume() const {
//  RE_NOT_IMPLEMENTED
  if (_shape != nullptr) {
    return _shape->volume();
  } else {
    return 0.0;
  }
}

inline const reMatrix reDistortedShape::computeInertia() const {
//  RE_NOT_IMPLEMENTED
  return reMatrix(1.0);
}

#endif
