/**
 * @file
 * Contains the definition for the reSolid abstract class
 */
#ifndef RE_SOLID_H
#define RE_SOLID_H

#include "react/Entities/reEnt.h"

/**
 * @ingroup entities
 * A type of reEnt which has a finite volume
 * 
 * @see reEnt
 */

class reSolid : public reEnt {
public:
  /** Default constructor does nothing */
  reSolid(reShape* shape);
  /** Default destructor does nothing */
  virtual ~reSolid();
};

inline reSolid::reSolid(reShape* shape) : reEnt(shape) {
  // do nothing
}

inline reSolid::~reSolid() {
  // do nothing
}

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

#endif
