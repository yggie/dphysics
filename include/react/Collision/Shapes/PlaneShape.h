/**
 * @file
 * Contains the definition of the re::PlaneShape class
 */
#ifndef RE_PLANE_SHAPE_H
#define RE_PLANE_SHAPE_H

#include "react/Collision/Shapes/reShape.h"

namespace re {
  class PlaneShape : public Plane, public reShape {
  public:
    PlaneShape(const re::vec3& normal, const re::vec3& point);
    PlaneShape(const re::vec3& normal, reFloat offset);

    Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;

    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    const re::vec3 center() const override;

    const re::vec3 randomPoint() const;

    bool containsPoint(const re::vec3& point) const override;

    bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;

    re::Plane::Location locationInPlane(const re::Plane& plane) const override;
  };

  inline PlaneShape::PlaneShape(const re::vec3& normal, const re::vec3& point) : Plane(normal, point), reShape() {
    // do nothing
  }

  inline PlaneShape::PlaneShape(const re::vec3& normal, reFloat offset) : Plane(normal, offset), reShape() {
    // do nothing
  }

  inline reShape::Type PlaneShape::type() const {
    return reShape::PLANE_SHAPE;
  }

  inline reUInt PlaneShape::numVerts() const {
    return 0;
  }

  inline const re::vec3 PlaneShape::vert(reUInt) const {
    return re::vec3();
  }

  inline reFloat PlaneShape::volume() const {
    return 0.0;
  }

  inline const re::mat3 PlaneShape::computeInertia() const {
    return re::mat3(1.0);
  }

  inline const re::vec3 PlaneShape::center() const {
    return normal() * offset();
  }

  inline bool PlaneShape::containsPoint(const re::vec3& point) const {
    return re::abs(re::dot(normal(), point) - offset()) < shell();
  }
};

#endif
