/**
 * @file
 * Contains the definition of the reEnt abstract class
 */
#ifndef RE_ENT_H
#define RE_ENT_H

#include "react/math.h"
#include "react/reWorld.h"

class reShape;

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
  
  /** Default constructor does nothing */
  reEnt();
  /** Default destructor does nothing */
  virtual ~reEnt();
  
  // must be implemented by the subclass
  virtual Type type() const = 0;
  virtual const reVector vel() const = 0;
  virtual const reMatrix rotVel() const = 0;
  
  // getter methods which are defined inline
  reShape* shape();
  const reShape* shape() const;
  const reVector pos() const;
  const reMatrix rot() const;
  reWorld* world();
  const reWorld* world() const;
  
  // setter methods which are defined inline
  void setPos(const reVector& position);
  void setPos(reFloat x, reFloat y, reFloat z);
  virtual void setShape(const reShape& shape);
  void setWorld(reWorld* world);
  virtual reEnt& at(const reVector& position) = 0;
  virtual reEnt& at(reFloat x, reFloat y, reFloat z) = 0;
  virtual reEnt& withShape(const reShape& shape) = 0;
  virtual reEnt& withMass(reFloat mass) = 0;
  virtual reEnt& withDensity(reFloat mass) = 0;
  
  /** a pointer to arbitrary data, defined by the user */
  void* userdata;
  
protected:
  /** The reWorld the reEnt exists in */
  reWorld* _world;
  /** The reEnt's reShape */
  reShape* _shape;
  /** The reEnt's position vector */
  reVector _vPos;
  /** The reEnt's rotation matrix */
  reMatrix _mRot;
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
 * @fn reEnt& reEnt::withreShape(const reShape& shape)
 * Set the reEnt's reShape, this method can be chained
 * 
 * @param shape The new reShape
 * @return A reference to the reEnt
 */

inline reEnt::reEnt() : userdata(nullptr), _world(nullptr), _shape(nullptr), _vPos(), _mRot() { }
inline reEnt::~reEnt() { }

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
 * Returns the position of the reEnt's center.
 * 
 * @return The position in 3D space in user defined units
 */

inline const reVector reEnt::pos() const {
  return _vPos;
}

/**
 * Returns the reEnt's rotation matrix
 * 
 * @return The rotation in matrix form
 */

inline const reMatrix reEnt::rot() const {
  return _mRot;
}

/**
 * Returns the reEnt's reWorld
 * 
 * @return Pointer to the reEnt's parent reWorld
 */

inline reWorld* reEnt::world() {
  return _world;
}

/**
 * Returns the reEnt's reWorld
 * 
 * const version
 * 
 * @return Pointer to the reEnt's parent reWorld
 */

inline const reWorld* reEnt::world() const {
  return _world;
}

/**
 * Set the reEnt's position
 * 
 * @param position The new position vector
 */

inline void reEnt::setPos(const reVector& position) {
  _vPos = position;
}

/**
 * Set the reEnt's position
 * 
 * @param x The new position x-coordinates
 * @param y The new position y-coordinates
 * @param z The new position z-coordinates
 */

inline void reEnt::setPos(reFloat x, reFloat y, reFloat z) {
  _vPos.set(x, y, z);
}

/**
 * Set the reEnt's reShape
 * 
 * @param shape The new reShape
 */

inline void reEnt::setShape(const reShape& shape) {
  _shape = &_world->copyOf(shape);
}

/**
 * Set the reEnt's reWorld, this method skips important steps in the reEnt
 * registration process. Instead, use reWorld::add(reEnt&)
 * 
 * @see reWorld::add(reEnt& entity)
 * @param world The new parent reWorld
 */

inline void reEnt::setWorld(reWorld* world) {
  _world = world;
}

#endif
