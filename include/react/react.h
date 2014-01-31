/**
 * @file
 * Contains all the necessary includes for the engine to work in an application
 */
#ifndef RE_REACT_H
#define RE_REACT_H

#include "react/common.h"
#include "react/math.h"

#include "react/Collision/Shapes/shapes.h"

#include "react/Entities/reEnt.h"
#include "react/Entities/reSolid.h"
#include "react/Entities/reRigidBody.h"

#include "react/Collision/reBroadPhase.h"
#include "react/Collision/reBSPTree.h"

#include "react/reWorld.h"

// DOCUMENTATION FOR DOXYGEN MODULES

/**
 * @defgroup memory Memory Management
 * This module implements custom memory management for the engine.
 */

/**
 * @defgroup entities Entities
 * This module defines the various physical entities which can exist in the
 * World
 */

/**
 * @defgroup shapes Shapes
 * This module defines the various geometric shapes which are supported by the
 * engine. Spatial queries eventually filter down to this module.
 */

/**
 * @defgroup dynamics Dynamics
 * This module defines the methods used to simulate and resolve interactions in
 * the physical simulation
 */

/**
 * @defgroup collision Collision
 * This module defines the various classes used to speed up collision queries.
 * Successful queries are passed on to the Shapes module
 */

/**
 * @defgroup utilities Utility
 * This module contains various utility classes used internally by many classes
 */

/**
 * @defgroup maths Maths
 * This module contains the representation for mathematical constructs and
 * operations used in the engine
 */

#endif
