#ifndef RE_ENT_H
#define RE_ENT_H

#include "react/vector.h"
#include "react/matrix.h"
#include "react/World.h"

namespace re {
  
  class Shape;
  
  /**
   * @ingroup entities
   * Represents a physical entity, which exists and interacts in the World.
   * 
   * @see World
   */
  
  class Ent {
  public:
    
    /** Defines unique types of entities which exists in the World */
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
    Ent();
    /** Default destructor does nothing */
    virtual ~Ent();
    
    // must be implemented by the subclass
    virtual Type type() const = 0;
    virtual const vec vel() const = 0;
    virtual const mat rotVel() const = 0;
    
    // getter methods which are defined inline
    Shape* shape();
    const Shape* shape() const;
    const vec pos() const;
    const mat rot() const;
    World* world();
    const World* world() const;
    
    // setter methods which are defined inline
    void setPos(const vec& position);
    void setPos(reFloat x, reFloat y, reFloat z);
    virtual void setShape(const Shape& shape);
    void setWorld(World* world);
    Ent& at(const vec& position);
    Ent& at(reFloat x, reFloat y, reFloat z);
    Ent& withShape(const Shape& shape);
    virtual Ent& withMass(reFloat mass);
    virtual Ent& withDensity(reFloat mass);
    
    /** a pointer to arbitrary data, defined by the user */
    void* userdata;
    
  protected:
    /** The World the Ent exists in */
    World* _world;
    /** The Ent's Shape */
    Shape* _shape;
    /** The Ent's position vector */
    vec _vPos;
    /** The Ent's rotation matrix */
    mat _mRot;
  };
  
  /**
   * @fn Type Ent::type() const
   * @brief Returns the Ent's type, it is unique for each base class
   * 
   * @return The entity type
   */
  
  /**
   * @fn const vec Ent::vel() const
   * @brief Returns the Ent's current velocity
   * 
   * @return The current velocity in 3D space in user-defined units
   */
  
  /**
   * @fn const mat Ent::rotVel() const
   * @brief Returns the Ent's rotational velocity matrix
   * 
   * @return The rotational velocity in matrix form
   */
  
  inline Ent::Ent() : userdata(nullptr), _world(nullptr), _shape(nullptr), _vPos(), _mRot() { }
  inline Ent::~Ent() { }
  
  /**
   * Returns the Shape of the object
   * 
   * @return The Shape of the object
   */
  
  inline Shape* Ent::shape() {
    return _shape;
  }
  
  /**
   * Returns the Shape of the object
   * 
   * const version
   * 
   * @return The Shape of the object
   */
  
  inline const Shape* Ent::shape() const {
    return _shape;
  }
  
  /**
   * Returns the position of the Ent's center.
   * 
   * @return The position in 3D space in user defined units
   */
  
  inline const vec Ent::pos() const {
    return _vPos;
  }
  
  /**
   * Returns the Ent's rotation matrix
   * 
   * @return The rotation in matrix form
   */
  
  inline const mat Ent::rot() const {
    return _mRot;
  }
  
  /**
   * Returns the Ent's World
   * 
   * @return Pointer to the Ent's parent World
   */
  
  inline World* Ent::world() {
    return _world;
  }
  
  /**
   * Returns the Ent's World
   * 
   * const version
   * 
   * @return Pointer to the Ent's parent World
   */
  
  inline const World* Ent::world() const {
    return _world;
  }
  
  /**
   * Set the Ent's position
   * 
   * @param position The new position vector
   */
  
  inline void Ent::setPos(const vec& position) {
    _vPos = position;
  }
  
  /**
   * Set the Ent's position
   * 
   * @param x The new position x-coordinates
   * @param y The new position y-coordinates
   * @param z The new position z-coordinates
   */
  
  inline void Ent::setPos(reFloat x, reFloat y, reFloat z) {
    _vPos.set(x, y, z);
  }
  
  /**
   * Set the Ent's Shape
   * 
   * @param shape The new Shape
   */
  
  inline void Ent::setShape(const Shape& shape) {
    _shape = &_world->copyOf(shape);
  }
  
  /**
   * Set the Ent's World, this method skips important steps in the Ent
   * registration process. Instead, use World::add(Ent&)
   * 
   * @see World::add(Ent& entity)
   * @param world The new parent World
   */
  
  inline void Ent::setWorld(World* world) {
    _world = world;
  }
  
  /**
   * Set the Ent's position, this method can be chained
   * 
   * @param position The new position vector
   * @return A reference to the Ent
   */
  
  inline Ent& Ent::at(const vec& position) {
    _vPos = position;
    return *this;
  }
  
  /**
   * Set the Ent's position, this method can be chained
   * 
   * @param x The new position x-coordinates
   * @param y The new position y-coordinates
   * @param z The new position z-coordinates
   * @return A reference to the Ent
   */
  
  inline Ent& Ent::at(reFloat x, reFloat y, reFloat z) {
    _vPos.set(x, y, z);
    return *this;
  }
  
  /**
   * Set the Ent's Shape, this method can be chained
   * 
   * @param shape The new Shape
   * @return A reference to the Ent
   */
  
  inline Ent& Ent::withShape(const Shape& shape) {
    setShape(shape);
    return *this;
  }
  
  /** This does nothing, only exists for compatibility */
  inline Ent& Ent::withMass(reFloat) { return *this; }
  /** This does nothing, only exists for compatibility */
  inline Ent& Ent::withDensity(reFloat) { return *this; }
}

#endif
