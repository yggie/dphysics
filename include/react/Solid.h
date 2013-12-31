#ifndef RE_SOLID_H
#define RE_SOLID_H

#include "react/Ent.h"

namespace re {
  
  /**
   * A type of Ent which has mass
   * 
   * @see Ent
   */
  
  class Solid : public Ent {
  public:
    /** Default constructor does nothing */
    Solid();
    /** Default destructor does nothing */
    virtual ~Solid();
    
    virtual reFloat mass() const = 0;
    virtual const mat inertia() const = 0;
    virtual reFloat density() const = 0;
  };
  
  /**
   * @fn reFloat Solid::mass() const
   * @brief Returns the Solid's total mass
   * 
   * @return The total mass in user defined units
   */
  
  /**
   * @fn const mat Solid::inertia() const
   * @brief Returns the Solid's inertia matrix
   * 
   * @return The inertia tensor in user-defined units
   */
  
  /**
   * @fn reFloat Solid::density() const
   * @brief Returns the Solid's density
   * 
   * @return The density in user-defined units
   */
  
  inline Solid::Solid() { }
  inline Solid::~Solid() { }
}

#endif
