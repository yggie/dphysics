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
  reSpatialQuery() : ID(re::globalQueryID++) { }
};

/**
 * The struct which defines spatial queries using rays
 */

struct reRayQuery : reSpatialQuery {
  reRayQuery() : reSpatialQuery(), origin(), dir() { }
  /** The ray origin */
  reVector origin;
  /** The ray direction */
  reVector dir;
};

struct reRayQueryResult {
  reRayQueryResult() : intersect(), normal(), distSq(0.0) { }
  reVector intersect;
  reVector normal;
  reFloat distSq;
};

/**
 * The struct which defines spatial queries using hyperplanes
 */

struct reHyperplaneQuery : reSpatialQuery {
  reHyperplaneQuery() : reSpatialQuery(), point(), dir() { }
  reVector point;
  reVector dir;
};

#endif
