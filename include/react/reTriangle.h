/**
 * @file
 * Contains the definition of the reTriangle class
 */
#ifndef RE_TRIANGLE_H
#define RE_TRIANGLE_H

#include "react/math.h"
#include "react/reShape.h"

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
  
  reVector& vert(int i);
  const reVector& vert(int i) const;
  
  reTriangle& withVertex(int i, const reVector& vert);
  
  reShape::Type type() const override;
  reFloat volume() const override;
  const reMatrix computeInertia() const;
  
  const reVector faceNorm() const;
  
  bool rayIntersect(const reVector& origin, const reVector& dir, reVector* intersect = nullptr, reVector* normal = nullptr) const override;
  
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

/**
 * Returns the vertex at the specified index
 * 
 * @param i The index of the vertex
 * @return The vertex at the index
 */

inline reVector& reTriangle::vert(int i) {
  return _verts[i];
}

/**
 * Returns the vertex at the specified index
 * 
 * Enforces constant constraint
 * 
 * @param i The index of the vertex
 * @return The vertex at the index
 */

inline const reVector& reTriangle::vert(int i) const {
  return _verts[i];
}

/**
 * Replaces a vertex in the reTriangle, this method can be chained
 * 
 * @param i The index of the vertex to be replaced
 * @param vert The new vertex
 * @return A reference to the reTriangle
 */

inline reTriangle& reTriangle::withVertex(int i, const reVector& vert) {
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
