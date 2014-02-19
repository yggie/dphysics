#ifndef RE_CONTACT_FILTER_H
#define RE_CONTACT_FILTER_H

#include "react/Entities/Entity.h"

namespace re {
  struct ContactFilter {
    bool filter(const Entity& A, const Entity& B);
  };

  inline bool ContactFilter::filter(const Entity& A, const Entity& B) {
    return (A.type() != Entity::STATIC) || (B.type() != Entity::STATIC);
  }
}

#endif
