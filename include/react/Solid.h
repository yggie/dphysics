#ifndef RE_SOLID_H
#define RE_SOLID_H

#include "react/Ent.h"

namespace re {
  
  /**
   * @ingroup entities
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
    
    virtual void setMass(reFloat mass) = 0;
    virtual void setDensity(reFloat density) = 0;
    
    void setShape(const Shape& shape);
    virtual Solid& withMass(reFloat mass) = 0;
    virtual Solid& withDensity(reFloat density) = 0;
    
  protected:
    virtual void updateInertia() = 0;
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
  
  /**
   * @fn void Solid::setMass(reFloat mass)
   * @brief Set the Solid's mass property. The mass and density properties are
   * not independent, therefore setting one or the other will override the
   * previous setting.
   * 
   * @param mass The mass in user-defined units
   */
  
  /**
   * @fn void Solid::setDensity(reFloat density)
   * @brief Set the Solid's density property. The mass and density properties are
   * not independent, therefore setting one or the other will override the
   * previous setting.
   * 
   * @param density The density in user-defined units
   */
  
  /**
   * @fn void Solid::updateInertia()
   * @brief Called internally to update the inertia tensor, either through a
   * change in shape, mass or density properties
   */
  
  /**
   * @fn Solid& Solid::withMass(reFloat mass)
   * Set the Solid's mass property, this method can be chained. The mass and
   * density properties are not independent, therefore setting one or the other
   * will override the previous setting.
   * 
   * @param mass The mass in user-defined units
   */
  
  /**
   * @fn Solid& Solid::withDensity(reFloat density)
   * Set the Solid's density property, this method can be chained. The mass and
   * density properties are not independent, therefore setting one or the other
   * will override the previous setting.
   * 
   * @param density The density in user-defined units
   */
  
  inline Solid::Solid() { }
  inline Solid::~Solid() { }
  
  inline void Solid::setShape(const Shape& shape) {
    Ent::setShape(shape);
    updateInertia();
  }
}

#endif
