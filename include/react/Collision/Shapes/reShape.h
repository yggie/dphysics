/**
 * @file
 * Contains the definition of the reShape abstract class
 */
#ifndef RE_SHAPE_H
#define RE_SHAPE_H

#include "react/common.h"
#include "react/math.h"
#include "react/Collision/reAABB.h"
#include "react/Collision/reSpatialQueries.h"

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
    /** A wrapper around shapes allow arbitrary transforms @see reProxyShape */
    PROXY
  };
  
  reShape();
  virtual ~reShape();
  
  // shape representation
  virtual Type type() const = 0;
  virtual reUInt numVerts() const = 0;
  virtual const re::vec vert(reUInt i) const = 0;
  virtual reFloat shell() const;
  
  // physical metrics
  virtual reFloat volume() const = 0;
  virtual const re::mat3 computeInertia() const = 0;
  virtual const re::vec offset() const;
  
  const reAABB& aabb() const;
  virtual void updateAABB(const re::mat3& parentRot);
  
  // utility methods
  virtual const re::vec3 randomPoint() const = 0;
  
  // collision queries
  bool containsPoint(const reTransform& transform, const re::vec3& point) const;
  
  virtual bool containsPoint(const re::vec3& point) const = 0;
  
  bool intersectsRay(const reTransform& transform, const reRayQuery& query, reRayQueryResult& result) const;
  
  virtual bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const = 0;
  
  virtual bool intersectsHyperplane(const reTransform& transform, const reHyperplaneQuery& query) const;
  
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
 * @fn const re::vec& reShape::vert(reUInt i) const
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
 * @fn bool reEnt::intersectsRay(const reEnt& parent, const re::vec& origin,
 * const re::vec& dir, re::vec* intersect = nullptr, re::vec* normal =
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

inline const re::vec reShape::offset() const {
  return re::vec(0.0, 0.0, 0.0);
}

#endif
