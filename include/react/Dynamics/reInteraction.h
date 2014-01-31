/**
 * @file
 * Contains the definition of the reInteraction abstract class
 */
#ifndef RE_INTERACTION_H
#define RE_INTERACTION_H

#include "react/Entities/reEnt.h"

class reInteraction {
public:
  virtual ~reInteraction() { }
  
  virtual void solve(reEnt& A, reEnt& B) = 0;
};

#endif
