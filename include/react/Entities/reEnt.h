/**
 * @file
 * Contains the definition of the reEnt abstract class
 */
#ifndef RE_ENT_H
#define RE_ENT_H

#include "react/math.h"
#include "react/Collision/reAABB.h"
#include "react/Utilities/reIntegrator.h"
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
    /** An entity with no volume */
    PARTICLE,
    /** A soft body entity with a deformable shape */
    SOFT,
    /** A massless physical entity, does not collide */
    FIELD
  };
  
  /** Disable default constructor */
  reEnt() = delete;
  /** Creates an entity from a parent reWorld object */
  reEnt(reShape* shape);
  /** Default destructor does nothing */
  virtual ~reEnt();
  
  // must be implemented by the subclass
  virtual Type type() const = 0;
  
  virtual void update(reIntegrator& integrator, reFloat dt) = 0;
  
  const reVector getAABBLowerCorner() const;
  const reVector getAABBUpperCorner() const;
  
  // getter methods
  reShape* shape();
  const reShape* shape() const;
  const reVector& pos() const;
  const reMatrix& rot() const;
  const reVector& vel() const;
  const reVector& angVel() const;
  const reTransform transform() const;
  const reVector center() const;
  reUInt id() const;
  virtual reFloat mass() const = 0;
  virtual const reMatrix& inertia() const = 0;
  virtual reFloat density() const = 0;
  
  // setter methods
  void setPos(const reVector& position);
  void setPos(reFloat x, reFloat y, reFloat z);
  virtual void setMass(reFloat) { } // stub
  virtual void setDensity(reFloat) { } // stub
  virtual reEnt& at(const reVector& position) = 0;
  virtual reEnt& at(reFloat x, reFloat y, reFloat z) = 0;
  virtual reEnt& withMass(reFloat mass) = 0;
  virtual reEnt& withDensity(reFloat mass) = 0;
  
  // collision queries
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const;
  
  bool intersectsHyperplane(const reHyperplaneQuery& query) const;
  bool intersectsHyperplane(const reVector& point, const reVector& dir) const;
  
  /** a pointer to arbitrary data, defined by the user */
  void* userdata;
  
protected:
  virtual void updateInertia() = 0;
  /** A unique identifier for the reEnt */
  const reUInt _id;
  /** The reEnt's reShape */
  reShape* _shape;
  /** The reEnt's transformation matrix */
  reTransform _transform;
  /** The reEnt's velocity vector */
  reVector _vel;
  /** The reEnt's angular velocity vector */
  reVector _angVel;
  
private:
  static reUInt globalEntID;
};

inline reEnt::reEnt(reShape* shape) : userdata(nullptr), _id(globalEntID++), _shape(shape), _transform() {
  // do nothing
}

inline reEnt::~reEnt() {
  // do nothing
}

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
 * Returns the reShape of the object. Enforces constant constraint
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

inline const reVector& reEnt::pos() const {
  return _transform.v;
}

/**
 * Returns the reEnt's rotation matrix
 * 
 * @return The rotation in matrix form
 */

inline const reMatrix& reEnt::rot() const {
  return _transform.m;
}

/**
 * @brief Returns the reEnt's current velocity
 * 
 * @return The current velocity in 3D space in user-defined units
 */

inline const reVector& reEnt::vel() const {
  return _vel;
}

/**
 * @brief Returns the reEnt's angular velocity vector
 * 
 * @return The angular velocity vector in user-defined units
 */

inline const reVector& reEnt::angVel() const {
  return _angVel;
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
 * Returns true if the reEnt intersects the ray specified
 * 
 * @param query The ray query struct
 * @param result The ray query result struct
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

/**
 * @fn Type reEnt::type() const
 * @brief Returns the reEnt's type, it is unique for each base class
 * 
 * @return The entity type
 */

/**
 * @fn void reEnt::update(reIntegrator& integrator, reFloat dt)
 * Moves the reEnt forward in time by the given time step using the specified
 * integration scheme
 * 
 * @param integrator The integration scheme
 * @param dt The time step in user defined units
 */

/**
 * @fn reFloat reEnt::mass() const
 * @brief Returns the reEnt's total mass
 * 
 * @return The total mass in user defined units
 */

/**
 * @fn const reMatrix reEnt::inertia() const
 * @brief Returns the reEnt's inertia reMatrixrix
 * 
 * @return The inertia tensor in user-defined units
 */

/**
 * @fn reFloat reEnt::density() const
 * @brief Returns the reEnt's density
 * 
 * @return The density in user-defined units
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
 * @fn reEnt& reEnt::withMass(reFloat mass)
 * Set the reEnt's mass property, this method can be chained. The mass and
 * density properties are not independent, therefore setting one or the other
 * will override the previous setting.
 * 
 * @param mass The mass in user-defined units
 */

/**
 * @fn reEnt& reEnt::withDensity(reFloat density)
 * Set the reEnt's density property, this method can be chained. The mass and
 * density properties are not independent, therefore setting one or the other
 * will override the previous setting.
 * 
 * @param density The density in user-defined units
 */

/**
 * @fn void reEnt::updateInertia()
 * @brief Called internally to update the inertia tensor, either through a
 * change in shape, mass or density properties
 */

#define RE_ENT_CHAINABLE_METHODS(KLASS) \
      KLASS& at(const reVector& position) override { setPos(position);return *this; } \
      KLASS& at(reFloat x, reFloat y, reFloat z) override { setPos(x, y, z);return *this; } \
      KLASS& withMass(reFloat mass) override { setMass(mass);return *this; } \
      KLASS& withDensity(reFloat density) override { setDensity(density);return *this; }

#endif
