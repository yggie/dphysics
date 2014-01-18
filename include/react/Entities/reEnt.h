/**
 * @file
 * Contains the definition of the reEnt abstract class
 */
#ifndef RE_ENT_H
#define RE_ENT_H

#include "react/math.h"
#include "react/reWorld.h"
#include "react/Collision/reAABB.h"
#include "react/Collision/Shapes/reShape.h"
#include "react/Collision/reSpatialQueries.h"

/**
 * @ingroup entities
 * Represents a physical entity, which exists and interacts in the reWorld.
 * 
 * @see reWorld
 */

class reEnt {
public:
  
  /** Defines unique types of entities which exists in the reWorld */
  enum Type {
    /** A rigid body entity with finite mass @see RigidBody */
    RIGID,
    /** A rigid body entity with infinite mass */
    STATIC,
    /** A physical entity with no volume */
    PARTICLE,
    /** A massless physical entity */
    FIELD
  };
  
  
  /** Disable default constructor */
  reEnt() = delete;
  /** Creates an entity from a parent reWorld object */
  reEnt(reWorld* world);
  /** Default destructor does nothing */
  virtual ~reEnt();
  
  // must be implemented by the subclass
  virtual Type type() const = 0;
  virtual const reVector vel() const = 0;
  virtual const reMatrix rotVel() const = 0;
  
  virtual void update(reFloat dt) = 0;
  
  const reVector getAABBLowerCorner() const;
  const reVector getAABBUpperCorner() const;
  
  // getter methods which are defined inline
  reShape* shape();
  const reShape* shape() const;
  const reVector pos() const;
  const reMatrix rot() const;
  const reTransform transform() const;
  virtual const reVector center() const;
  reUInt id() const;
  
  // setter methods which are defined inline
  void setPos(const reVector& position);
  void setPos(reFloat x, reFloat y, reFloat z);
  virtual void setShape(const reShape& shape);
  virtual reEnt& at(const reVector& position) = 0;
  virtual reEnt& at(reFloat x, reFloat y, reFloat z) = 0;
  virtual reEnt& withShape(const reShape& shape) = 0;
  virtual reEnt& withMass(reFloat mass) = 0;
  virtual reEnt& withDensity(reFloat mass) = 0;
  
  // collision queries
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const;
  
  bool intersectsHyperplane(const reHyperplaneQuery& query) const;
  bool intersectsHyperplane(const reVector& point, const reVector& dir) const;
  
  /** a pointer to arbitrary data, defined by the user */
  void* userdata;
  
protected:
  /** A reference to the parent reWorld object */
  reWorld& _world;
  /** A unique identifier for the reEnt */
  const reUInt _id;
  /** The reEnt's reShape */
  reShape* _shape;
  /** The reEnt's transformation matrix */
  reTransform _transform;
  
  static reUInt globalEntID;
};

/**
 * @fn Type reEnt::type() const
 * @brief Returns the reEnt's type, it is unique for each base class
 * 
 * @return The entity type
 */

/**
 * @fn const reVector reEnt::vel() const
 * @brief Returns the reEnt's current velocity
 * 
 * @return The current velocity in 3D space in user-defined units
 */

/**
 * @fn const reMatrix reEnt::rotVel() const
 * @brief Returns the reEnt's rotational velocity matrix
 * 
 * @return The rotational velocity in matrix form
 */

/**
 * @fn reEnt& reEnt::at(const reVector& pos)
 * Set the reEnt's position, this method can be chained
 * 
 * @param position The new position vector
 * @return A reference to the reEnt
 */

/**
 * @fn reEnt& reEnt::at(reFloat x, reFloat y, reFloat z)
 * Set the reEnt's position, this method can be chained
 * 
 * @param x The new position x-coordinates
 * @param y The new position y-coordinates
 * @param z The new position z-coordinates
 * @return A reference to the reEnt
 */

/**
 * @fn reEnt& reEnt::withShape(const reShape& shape)
 * Set the reEnt's reShape, this method can be chained
 * 
 * @param shape The new reShape
 * @return A reference to the reEnt
 */

/**
 * Notifies the reEnt that it should update its AABB
 */

//inline void reEnt::update() {
//  if (shape() != nullptr) {
//    shape()->updateAABB(_transform.m);
//  }
//}

inline const reVector reEnt::getAABBLowerCorner() const {
  if (shape() != nullptr) {
    return center() - shape()->aabb().dimens();
  } else {
    return _transform.v;
  }
}

inline const reVector reEnt::getAABBUpperCorner() const {
  if (shape() != nullptr) {
    return center() + shape()->aabb().dimens();
  } else {
    return _transform.v;
  }
}

/**
 * Returns the reShape of the object
 * 
 * @return The reShape of the object
 */

inline reShape* reEnt::shape() {
  return _shape;
}

/**
 * Returns the reShape of the object
 * 
 * const version
 * 
 * @return The reShape of the object
 */

inline const reShape* reEnt::shape() const {
  return _shape;
}

/**
 * Returns the position of the reEnt.
 * 
 * @return The position in 3D space in user defined units
 */

inline const reVector reEnt::pos() const {
  return _transform.v;
}

/**
 * Returns the reEnt's rotation matrix
 * 
 * @return The rotation in matrix form
 */

inline const reMatrix reEnt::rot() const {
  return _transform.m;
}

/**
 * Returns the reEnt's reTransform
 * 
 * @return The transformation in matrix form
 */

inline const reTransform reEnt::transform() const {
  return _transform;
}

/**
 * Returns the position of the center of the reShape associated with the reEnt.
 * This differs from reEnt::pos() when the shape has an offset
 * 
 * @return The centroid of the reShape in user-defined units
 */

inline const reVector reEnt::center() const {
  if (shape() != nullptr) {
    return _transform.v + shape()->offset();
  } else {
    return _transform.v;
  }
}

/**
 * Returns the unique identifier for the reEnt
 * 
 * @return A unique identifier for the reEnt
 */

inline reUInt reEnt::id() const {
  return _id;
}

/**
 * Set the reEnt's position
 * 
 * @param position The new position vector
 */

inline void reEnt::setPos(const reVector& position) {
  _transform.v = position;
}

/**
 * Set the reEnt's position
 * 
 * @param x The new position x-coordinates
 * @param y The new position y-coordinates
 * @param z The new position z-coordinates
 */

inline void reEnt::setPos(reFloat x, reFloat y, reFloat z) {
  _transform.v.set(x, y, z);
}

/**
 * Set the reEnt's reShape
 * 
 * @param shape The new reShape
 */

inline void reEnt::setShape(const reShape& shape) {
  _shape = &_world.copyOf(shape);
}

/**
 * Returns true if the reEnt intersects the ray specified
 * 
 * @param origin The ray origin
 * @param dir The ray direction
 * @param intersect An optional argument which is filled with the intersect
 *                  point
 * @param normal An optional argument which is filled with the intersect norm
 * @param queryID Used internally to avoid redundant queries
 * @return True if the ray intersects
 */

inline bool reEnt::intersectsRay(const reRayQuery& query, reRayQueryResult& result) const {
  if (_shape != nullptr) {
    return _shape->intersectsRay(_transform, query, result);
  }
  return false;
}

inline bool reEnt::intersectsHyperplane(const reHyperplaneQuery& query) const {
  if (_shape != nullptr) {
    return _shape->intersectsHyperplane(_transform, query);
  } else {
    return (_transform.v - query.point).dot(query.dir) > 0.0;
  }
}

inline bool reEnt::intersectsHyperplane(const reVector& point, const reVector& dir) const {
  if (_shape != nullptr) {
    reHyperplaneQuery query;
    query.point = point;
    query.dir = dir;
    return _shape->intersectsHyperplane(_transform, query);
  } else {
    return (_transform.v - point).dot(dir) > 0.0;
  }
}

#endif
