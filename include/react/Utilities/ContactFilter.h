#ifndef RE_CONTACT_FILTER_H
#define RE_CONTACT_FILTER_H

#include "react/Entities/reEnt.h"

namespace re {
  struct ContactFilter {
    bool filter(const reEnt& A, const reEnt& B);
  };

  inline bool ContactFilter::filter(const reEnt& A, const reEnt& B) {
    return (A.type() != reEnt::STATIC) || (B.type() != reEnt::STATIC);
  }
}

#endif
