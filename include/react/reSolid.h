/**
 * @file
 * Contains the definition for the reSolid abstract class
 */
#ifndef RE_SOLID_H
#define RE_SOLID_H

#include "react/reEnt.h"

/**
 * @ingroup entities
 * A type of reEnt which has mass
 * 
 * @see reEnt
 */

class reSolid : public reEnt {
public:
  /** Default constructor does nothing */
  reSolid();
  /** Default destructor does nothing */
  virtual ~reSolid();
  
  virtual reFloat mass() const = 0;
  virtual const reMatrix inertia() const = 0;
  virtual reFloat density() const = 0;
  
  virtual void setMass(reFloat mass) = 0;
  virtual void setDensity(reFloat density) = 0;
  
  void setShape(const reShape& shape);
  virtual reSolid& withMass(reFloat mass) = 0;
  virtual reSolid& withDensity(reFloat density) = 0;
  
protected:
  virtual void updateInertia() = 0;
};

/**
 * @fn reFloat reSolid::mass() const
 * @brief Returns the reSolid's total mass
 * 
 * @return The total mass in user defined units
 */

/**
 * @fn const reMatrix reSolid::inertia() const
 * @brief Returns the reSolid's inertia reMatrixrix
 * 
 * @return The inertia tensor in user-defined units
 */

/**
 * @fn reFloat reSolid::density() const
 * @brief Returns the reSolid's density
 * 
 * @return The density in user-defined units
 */

/**
 * @fn void reSolid::setMass(reFloat mass)
 * @brief Set the reSolid's mass property. The mass and density properties are
 * not independent, therefore setting one or the other will override the
 * previous setting.
 * 
 * @param mass The mass in user-defined units
 */

/**
 * @fn void reSolid::setDensity(reFloat density)
 * @brief Set the reSolid's density property. The mass and density properties are
 * not independent, therefore setting one or the other will override the
 * previous setting.
 * 
 * @param density The density in user-defined units
 */

/**
 * @fn void reSolid::updateInertia()
 * @brief Called internally to update the inertia tensor, either through a
 * change in shape, mass or density properties
 */

/**
 * @fn reSolid& reSolid::withMass(reFloat mass)
 * Set the reSolid's mass property, this method can be chained. The mass and
 * density properties are not independent, therefore setting one or the other
 * will override the previous setting.
 * 
 * @param mass The mass in user-defined units
 */

/**
 * @fn reSolid& reSolid::withDensity(reFloat density)
 * Set the reSolid's density property, this method can be chained. The mass and
 * density properties are not independent, therefore setting one or the other
 * will override the previous setting.
 * 
 * @param density The density in user-defined units
 */

inline reSolid::reSolid() { }
inline reSolid::~reSolid() { }

inline void reSolid::setShape(const reShape& shape) {
  reEnt::setShape(shape);
  updateInertia();
}

#endif
