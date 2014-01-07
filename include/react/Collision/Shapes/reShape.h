/**
 * @file
 * Contains the definition of the reShape abstract class
 */
#ifndef RE_SHAPE_H
#define RE_SHAPE_H

#include "react/common.h"
#include "react/math.h"
#include "react/Collision/reAABB.h"

/**
 * @ingroup shapes
 * Represents an shape with defined geometric qualities
 */

class reShape {
public:
  
  /** The types of shapes supported by the engine */
  enum Type {
    /** A sphere @see reSphere */
    SPHERE,
    /** A rectangle @see reRectangle */
    RECTANGLE,
    /** A compound shape type */
    COMPOUND,
    /** A planar triangle @see reTriangle */
    TRIANGLE,
    /** A wrapper around shapes allow arbitrary transforms @see reDistortedShape */
    DISTORTED
  };
  
  reShape();
  virtual ~reShape();
  
  // shape representation
  virtual Type type() const = 0;
  virtual reUInt numVerts() const = 0;
  virtual const reVector vert(reUInt i) const = 0;
  virtual reFloat shell() const;
  
  // physical metrics
  virtual reFloat volume() const = 0;
  virtual const reMatrix computeInertia() const = 0;
  virtual const reVector offset() const;
  
  const reAABB& aabb() const;
  virtual void updateAABB(const reMatrix& parentRot);
  
  // collision queries
  virtual bool intersectsRay(
    const reTMatrix& transform,
    const reVector& origin,
    const reVector& dir,
    reVector* intersect = nullptr,
    reVector* normal = nullptr
  ) const;
  
  virtual bool intersectsRay(
    const reVector& origin,
    const reVector& dir,
    reVector* intersect = nullptr,
    reVector* normal = nullptr
  ) const = 0;
  
  virtual bool intersectsHyperplane(
    const reTMatrix& transform,
    const reVector& point,
    const reVector& dir
  ) const;
  
protected:
  reAABB _aabb;
  reFloat _shell;
};

/**
 * @fn Type reShape::type() const
 * @brief Returns the reShape's type
 * 
 * @return The reShape's type
 */

/**
 * @fn const reVector& reShape::vert(reUInt i) const
 * Returns the vertex at the specified index
 * 
 * Enforces constant constraint
 * 
 * @param i The index of the vertex
 * @return The vertex at the index specified
 */

/**
 * @fn reFloat reShape::volume() const
 * @brief Returns the volume of the shape
 * 
 * @return The calculated volume in user-defined units
 */

/**
 * @fn const mat reShape::computeInertia() const
 * @brief Returns the inertia tensor assuming unit mass
 * 
 * @return The inertia tensor in user-defined units
 */

/**
 * @fn bool reEnt::intersectsRay(const reEnt& parent, const reVector& origin,
 * const reVector& dir, reVector* intersect = nullptr, reVector* normal =
 * nullptr) const
 * @brief Returns true if the ray specified intersects with the shape
 * 
 * @param parent The parent reEnt
 * @param origin The ray origin
 * @param dir The ray direction
 * @param intersect An optional argument which is filled with the intersect point
 * @param normal An optional argument which is filled with the intersect norm
 * @return True if the ray intersects
 */

inline reShape::reShape() : _shell(RE_FP_TOLERANCE) { }
inline reShape::~reShape() { }

/**
 * Returns the shell layer thickness used in collision detection
 * 
 * @return The shell layer thickness in user-defined units
 */

inline reFloat reShape::shell() const {
  return _shell;
}

inline const reAABB& reShape::aabb() const {
  return _aabb;
}

/**
 * Returns the offset required from the reEnt position to reach the centroid
 * 
 * @return The position of the centroid in model space
 */

inline const reVector reShape::offset() const {
  return reVector(0.0, 0.0, 0.0);
}

#endif
