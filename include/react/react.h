/**
 * @file
 * Contains all the necessary includes for the engine to work in an application
 */
#ifndef RE_REACT_H
#define RE_REACT_H

#include "react/common.h"
#include "react/math.h"

#include "react/reShape.h"
#include "react/reSphere.h"
#include "react/reTriangle.h"
#include "react/reDistortedShape.h"

#include "react/reEnt.h"
#include "react/reSolid.h"
#include "react/reRigidBody.h"

#include "react/broadphase/reBroadPhase.h"
#include "react/broadphase/reKDTree.h"

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
 * engine
 */

/**
 * @defgroup broadphase Broad Phase
 * This module defines the various implementations for a broad phase collision
 * detection system
 */

#endif
