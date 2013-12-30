#ifndef RE_ENTITY_H
#define RE_ENTITY_H

#include "react/vector.h"
#include "react/matrix.h"

namespace re {
  
  /**
   * Represents a physical entity, which is managed by a World object
   * 
   * @see World
   */
  
  class Entity {
  public:
    
    enum Type {
      RIGID,
      STATIC,
      PARTICLE,
      FIELD
    };
    
    Entity();
    virtual ~Entity();
    
    virtual Type type() const = 0;
    
    // getter methods
    virtual const vec pos() const = 0;
    virtual const mat rot() const = 0;
    virtual const vec vel() const = 0;
    virtual const mat rotVel() const = 0;
    
    /** a pointer to arbitrary data, defined by the user */
    void* userdata;
  };
  
  inline Entity::Entity() : userdata(nullptr) { }
  inline Entity::~Entity() { }
}

#endif
