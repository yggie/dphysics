/**
 * @file
 * Contains the definition of the re::Entity abstract class
 */
#ifndef RE_ENTITY_H
#define RE_ENTITY_H

#include "react/math.h"
#include "react/Math/Integrator.h"
#include "react/Collision/Shapes/reShape.h"
#include "react/Collision/reSpatialQueries.h"

namespace re {
  class Plane;
  enum Location;

  /**
   * @ingroup entities
   * Represents a physical entity, which exists and interacts in the reWorld.
   * 
   * @see reWorld
   */

  class Entity {
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
    Entity() = delete;
    /** Creates an entity from a parent reWorld object */
    Entity(reShape& shape);
    /** Default destructor does nothing */
    virtual ~Entity();

    //=====================================================
    //    IDENTIFICATION
    //=====================================================
 
    virtual Type type() const = 0;
    re::ID id() const;

    //=====================================================
    //    PHYSICAL GEOMETRY
    //=====================================================

    // getters
    reShape& shape();
    const reShape& shape() const;

    //=====================================================
    //    PHYSICAL STATE
    //=====================================================

    // getters
    const re::vec3& pos() const;
    virtual const re::mat3 rot() const = 0;
    virtual const re::quat& orient() const = 0;
    virtual const re::vec3& vel() const = 0;
    virtual const re::vec3& angVel() const = 0;
    const re::Transform transform() const;
    const re::vec3 center() const;

    // setters
    void setPos(const re::vec3& position);
    void setPos(reFloat x, reFloat y, reFloat z);
    virtual void setVel(const re::vec3& vel) = 0;
    virtual void setVel(reFloat x, reFloat y, reFloat z) = 0;
    virtual void setAngVel(const re::vec3&) = 0;
    virtual void setAngVel(reFloat, reFloat, reFloat) = 0; 
    virtual void setFacing(const re::vec3& dir, const re::vec3& up = re::vec3(0.0, 0.0, 1.0)) = 0; 

    // operations
    virtual void advance(re::Integrator& integrator, reFloat dt) = 0;
    virtual void addImpulse(const re::vec3& impulse) = 0;

    //=====================================================
    //    PHYSICAL PROPERTIES
    //=====================================================

    // getters
    virtual reFloat mass() const = 0;
    virtual reFloat massInv() const = 0;
    virtual const re::mat3 inertia() const = 0;
    virtual const re::mat3& inertiaInv() const = 0;
    virtual reFloat density() const = 0;
    virtual reFloat restitution() const = 0;
    virtual reFloat friction() const = 0;
    virtual reFloat resistance() const = 0;

    // setter methods
    virtual void setMass(reFloat mass) = 0;
    virtual void setDensity(reFloat density) = 0;
    virtual void setRestitution(reFloat restitution) = 0;
    virtual void setFriction(reFloat friction) = 0;
    virtual void setResistance(reFloat resistance) = 0;

    //=====================================================
    //    METHODS FOR CHAINING
    //=====================================================

    virtual Entity& at(const re::vec3& position) = 0;
    virtual Entity& at(reFloat x, reFloat y, reFloat z) = 0;
    virtual Entity& facing(const re::vec3& dir, const re::vec3& up) = 0;
    virtual Entity& movingAt(const re::vec3& velocity) = 0;
    virtual Entity& movingAt(reFloat vx, reFloat vy, reFloat vz) = 0;
    virtual Entity& rotatingWith(reFloat wx, reFloat wy, reFloat wz) = 0;
    virtual Entity& withMass(reFloat mass) = 0;
    virtual Entity& withDensity(reFloat mass) = 0;
    virtual Entity& withRestitution(reFloat restitution) = 0;
    virtual Entity& withFriction(reFloat friction) = 0;
    virtual Entity& withResistance(reFloat resistance) = 0;

    //=====================================================
    //    COLLISION QUERIES
    //=====================================================

    bool intersects(const re::Ray& ray, re::Intersect& intersect) const;
    re::Location relativeToPlane(const re::Plane& plane);

    /** a pointer to arbitrary data, defined by the user */
    void* userdata;

  protected:
    /** A unique identifier for the re::Entity */
    const re::ID _id;
    /** The entity's reShape */
    reShape& _shape;
    /** The entity's position vector */
    re::vec3 _pos;

  private:
    static re::ID globalEntID;
  };

  inline Entity::Entity(reShape& shape) : userdata(nullptr), _id(globalEntID++), _shape(shape), _pos() {
    // do nothing
  }

  inline Entity::~Entity() {
    // do nothing
  }

  /**
   * Returns the reShape of the object
   * 
   * @return The reShape of the object
   */

  inline reShape& Entity::shape() {
    return _shape;
  }

  /**
   * Returns the reShape of the object. Enforces constant constraint
   * 
   * @return The reShape of the object
   */

  inline const reShape& Entity::shape() const {
    return _shape;
  }

  /**
   * Returns the position of the re::Entity.
   * 
   * @return The position in 3D space in user defined units
   */

  inline const re::vec3& Entity::pos() const {
    return _pos;
  }

  /**
   * const re::vec3& re::Entity::vel() const
   * @brief Returns the re::Entity's current velocity
   * 
   * @return The current velocity in 3D space in user-defined units
   */

  /**
   * Returns the entity's transformation matrix
   * 
   * @return The transformation in matrix form
   */

  inline const re::Transform Entity::transform() const {
    return re::Transform(rot(), _pos);
  }

  /**
   * Returns the position of the center of the reShape associated with the re::Entity.
   * This differs from Entity::pos() when the shape has an offset
   * 
   * @return The centroid of the reShape in user-defined units
   */

  inline const re::vec3 Entity::center() const {
    return _pos + shape().center();
  }

  /**
   * Returns the unique identifier for the re::Entity
   * 
   * @return A unique identifier for the re::Entity
   */

  inline re::ID Entity::id() const {
    return _id;
  }

  /**
   * Set the re::Entity's position
   * 
   * @param position The new position vector
   */

  inline void Entity::setPos(const re::vec3& position) {
    _pos = position;
  }

  /**
   * Set the re::Entity's position
   * 
   * @param x The new position x-coordinates
   * @param y The new position y-coordinates
   * @param z The new position z-coordinates
   */

  inline void Entity::setPos(reFloat x, reFloat y, reFloat z) {
    _pos.set(x, y, z);
  }

  /**
   * Set the re::Entity's velocity
   * 
   * @param velocity The new velocity vector
   */

  /**
   * Set the re::Entity's velocity
   * 
   * @param x The new velocity in the x-direction
   * @param y The new velocity in the y-direction
   * @param z The new velocity in the z-direction
   */

  /**
   * @fn Type Entity::type() const
   * @brief Returns the re::Entity's type, it is unique for each base class
   * 
   * @return The entity type
   */

  /**
   * @fn void Entity::advance(reIntegrator& integrator, reFloat dt)
   * Moves the re::Entity forward in time by the given time step using the specified
   * integration scheme
   * 
   * @param integrator The integration scheme
   * @param dt The time step in user defined units
   */

  /**
   * @fn const re::mat3 Entity::rot() const
   * Returns the re::Entity's rotation matrix
   * 
   * @return The rotation in matrix form
   */

  /**
   * @fn const reQuaternion& Entity::quat() const
   * Returns the re::Entity's rotation represented as a quaternion
   * 
   * @return The rotation in quaternion form
   */

  /**
   * @fn const re::vec3& Entity::angVel() const
   * Returns the reSolid's angular velocity vector
   * 
   * @return The angular velocity vector in user-defined units
   */

  /**
   * @fn reFloat Entity::mass() const
   * @brief Returns the re::Entity's total mass
   * 
   * @return The total mass in user defined units
   */

  /**
   * @fn const re::mat3 Entity::inertia() const
   * @brief Returns the re::Entity's inertia re::mat3rix
   * 
   * @return The inertia tensor in user-defined units
   */

  /**
   * @fn reFloat Entity::density() const
   * @brief Returns the re::Entity's density
   * 
   * @return The density in user-defined units
   */

  /**
   * @fn re::Entity& Entity::at(const re::vec3& pos)
   * Set the re::Entity's position, this method can be chained
   * 
   * @param position The new position vector
   * @return A reference to the re::Entity
   */

  /**
   * @fn re::Entity& Entity::at(reFloat x, reFloat y, reFloat z)
   * Set the re::Entity's position, this method can be chained
   * 
   * @param x The new position x-coordinates
   * @param y The new position y-coordinates
   * @param z The new position z-coordinates
   * @return A reference to the re::Entity
   */

  /**
   * @fn void Entity::setMass(reFloat mass)
   * @brief Set the re::Entity's mass property. The mass and density properties are
   * not independent, therefore setting one or the other will override the
   * previous setting.
   * 
   * @param mass The mass in user-defined units
   */

  /**
   * @fn void Entity::setDensity(reFloat density)
   * @brief Set the re::Entity's density property. The mass and density properties are
   * not independent, therefore setting one or the other will override the
   * previous setting.
   * 
   * @param density The density in user-defined units
   */

  /**
   * @fn re::Entity& Entity::withMass(reFloat mass)
   * Set the re::Entity's mass property, this method can be chained. The mass and
   * density properties are not independent, therefore setting one or the other
   * will override the previous setting.
   * 
   * @param mass The mass in user-defined units
   */

  /**
   * @fn re::Entity& Entity::withDensity(reFloat density)
   * Set the re::Entity's density property, this method can be chained. The mass and
   * density properties are not independent, therefore setting one or the other
   * will override the previous setting.
   * 
   * @param density The density in user-defined units
   */

  /**
   * @fn void Entity::updateInertia()
   * @brief Called internally to update the inertia tensor, either through a
   * change in shape, mass or density properties
   */

}

#define EXTENDS_ENTITY(KLASS) \
      KLASS& at(const re::vec3& position) override { setPos(position);return *this; } \
      KLASS& at(reFloat x, reFloat y, reFloat z) override { setPos(x, y, z);return *this; } \
      KLASS& withMass(reFloat mass) override { setMass(mass);return *this; } \
      KLASS& withDensity(reFloat density) override { setDensity(density);return *this; } \
      KLASS& facing(const re::vec3& dir, const re::vec3& up) override { setFacing(dir, up); return *this; } \
      KLASS& movingAt(const re::vec3& velocity) override { setVel(velocity); return *this; } \
      KLASS& movingAt(reFloat vx, reFloat vy, reFloat vz) override { setVel(re::vec3(vx, vy, vz)); return *this; } \
      KLASS& rotatingWith(reFloat wx, reFloat wy, reFloat wz) override { setAngVel(wx, wy, wz); return *this; } \
      KLASS& withRestitution(reFloat restitution) override { setRestitution(restitution); return *this; } \
      KLASS& withFriction(reFloat friction) override { setFriction(friction); return *this; } \
      KLASS& withResistance(reFloat resistance) override { setResistance(resistance); return *this; }

#endif
