/**
 * @file
 * Contains the definition of the reInteraction abstract class
 */
#ifndef RE_INTERACTION_H
#define RE_INTERACTION_H

#include "react/Entities/Entity.h"

class reInteraction {
public:
  virtual ~reInteraction() { }
  
  virtual void solve(re::Entity& A, re::Entity& B) = 0;
};

#endif
