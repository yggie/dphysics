/**
 * @file
 * Contains the definition of the reTriangle class
 */
#ifndef RE_TRIANGLE_H
#define RE_TRIANGLE_H

#include "react/Collision/Shapes/reShape.h"

/**
 * @ingroup shapes
 * Represents a planar triangle with arbitrary vertices
 */

class reTriangle : public reShape {
public:
  /** Default constructor does nothing */
  reTriangle();
  reTriangle(const reTriangle& other);
  reTriangle(const reVector& a, const reVector& b, const reVector& c);
  /** Destructor does nothing */
  ~reTriangle();
  
  reTriangle& withVertex(reUInt i, const reVector& vert);
  
  // shape representation
  reShape::Type type() const override;
  reUInt numVerts() const override;
  const reVector vert(reUInt i) const override;
  const reVector offset() const override;
  
  // physical metrics
  reFloat volume() const override;
  const reMatrix computeInertia() const;
  
  const reVector faceNorm() const;
  
  bool intersectsRay(
    const reVector& origin,
    const reVector& dir,
    reVector* intersect = nullptr,
    reVector* normal = nullptr
  ) const override;
  
private:
  reVector _verts[3];
};

inline reTriangle::reTriangle() : _verts() {
  // do nothing
}

/**
 * Copy constructor
 * 
 * @param other The other reTriangle to copy
 */

inline reTriangle::reTriangle(const reTriangle& other) : _verts() {
  for (int i = 0; i < 3; i++) {
    _verts[i] = other._verts[i];
  }
}

/**
 * Creates a reTriangle with the vertices specified
 * 
 * @param a The first vertex
 * @param b The second vertex
 * @param c The third vertex
 */

inline reTriangle::reTriangle(const reVector& a, const reVector& b, const reVector& c) {
  _verts[0] = a;
  _verts[1] = b;
  _verts[2] = c;
}

inline reTriangle::~reTriangle() {
  // do nothing
}

inline reUInt reTriangle::numVerts() const {
  return 3;
}

inline const reVector reTriangle::vert(reUInt i) const {
  return _verts[i];
}

inline const reVector reTriangle::offset() const {
  return (_verts[0] + _verts[1] + _verts[2]) / 3.0;
}

/**
 * Replaces a vertex in the reTriangle, this method can be chained
 * 
 * @param i The index of the vertex to be replaced
 * @param vert The new vertex
 * @return A reference to the reTriangle
 */

inline reTriangle& reTriangle::withVertex(reUInt i, const reVector& vert) {
  _verts[i] = vert;
  return *this;
}

/**
 * Returns the type identifier associated with this reShape
 * 
 * @return Always reShape::TRIANGLE
 */

inline reShape::Type reTriangle::type() const {
  return reShape::TRIANGLE;
}

/**
 * Returns the volume of the reTriangle, which is always 0
 * 
 * @return 0
 */

inline reFloat reTriangle::volume() const {
  return 0.0;
}

inline const reMatrix reTriangle::computeInertia() const {
//  RE_NOT_IMPLEMENTED
  return reMatrix(1.0f);
}

inline const reVector reTriangle::faceNorm() const {
  return (_verts[0] - _verts[1]).cross(_verts[2] - _verts[1]).normalized();
}

#endif
