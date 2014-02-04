/**
 * @file
 * Contains various definitions for spatial query structs
 */
#ifndef RE_SPATIALQUERIES_H
#define RE_SPATIALQUERIES_H

#include "react/common.h"
#include "react/math.h"

class reEnt;

namespace re {
  extern reUInt globalQueryID;  // defined in common.h
};

/**
 * Base class for all query structs
 */

struct reSpatialQuery {
  /** A unique query ID to avoid redundant queries */
  const reUInt ID;
protected:
  // ensures the ID is never zero
  reSpatialQuery() : ID(
    (++re::globalQueryID == 0) ? ++re::globalQueryID : re::globalQueryID
  ) { }
};

/**
 * The struct which defines spatial queries using rays
 */

struct reRayQuery : reSpatialQuery {
  reRayQuery() : reSpatialQuery(), origin(), dir() { }
  /** The ray origin */
  re::vec origin;
  /** The ray direction */
  re::vec dir;
};

struct reRayQueryResult {
  reRayQueryResult() : intersect(), normal(), distSq(RE_INFINITY) { }
  re::vec intersect;
  re::vec normal;
  reFloat distSq;
};

namespace re {
  struct PlaneQuery {
    enum FastResult {
      FRONT,
      INTERSECTS,
      BEHIND
    };
  };
}

#endif
