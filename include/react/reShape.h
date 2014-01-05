/**
 * @file
 * Contains the definition of the reShape abstract class
 */
#ifndef RE_SHAPE_H
#define RE_SHAPE_H

#include "react/common.h"

class reTMatrix;
class reMatrix;
class reVector;

/**
 * @ingroup shapes
 * Represents an shape with defined geometric qualities
 */

class reShape {
public:
  
  /** The types of shapes supported by the engine */
  enum Type {
    /** A sphere @see reSphere */
    SPHERE,
    /** A rectangle @see reRectangle */
    RECTANGLE,
    /** A compound shape type */
    COMPOUND,
    /** A planar triangle @see reTriangle */
    TRIANGLE,
    /** A special extension of shapes allow arbitrary operations @see reDistortedShape */
    DISTORTED
  };
  
  reShape();
  virtual ~reShape();
  
  virtual Type type() const = 0;
  virtual reFloat volume() const = 0;
  virtual const reMatrix computeInertia() const = 0;
  
  virtual bool rayIntersect(const reTMatrix& transform, const reVector& origin, const reVector& dir, reVector* intersect = nullptr, reVector* normal = nullptr) const;
  virtual bool rayIntersect(const reVector& origin, const reVector& dir, reVector* intersect = nullptr, reVector* normal = nullptr) const = 0;
};

/**
 * @fn Type reShape::type() const
 * @brief Returns the reShape's type
 * 
 * @return The reShape's type
 */

/**
 * @fn reFloat reShape::volume() const
 * @brief Returns the volume of the shape
 * 
 * @return The calculated volume in user-defined units
 */

/**
 * @fn const mat reShape::computeInertia() const
 * @brief Returns the inertia tensor assuming unit mass
 * 
 * @return The inertia tensor in user-defined units
 */

/**
 * @fn bool reEnt::rayIntersect(const reEnt& parent, const reVector& origin,
 * const reVector& dir, reVector* intersect = nullptr, reVector* normal =
 * nullptr) const
 * @brief Returns true if the ray specified intersects with the shape
 * 
 * @param parent The parent reEnt
 * @param origin The ray origin
 * @param dir The ray direction
 * @param intersect An optional argument which is filled with the intersect point
 * @param normal An optional argument which is filled with the intersect norm
 * @return True if the ray intersects
 */

inline reShape::reShape() { }
inline reShape::~reShape() { }

#endif
