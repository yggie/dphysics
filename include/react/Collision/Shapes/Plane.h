#ifndef RE_PLANE_H
#define RE_PLANE_H

#include "react/Collision/Shapes/reShape.h"

namespace re {
  class Plane : public reShape {
  public:
    Plane(const re::vec3& normal, const re::vec3& point);
    Plane(const re::vec3& normal, reFloat offset);

    // shape representation
    Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;
    reFloat shell() const override;

    // physical metrics
    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    const re::vec3 offset() const override;

    // utility methods
    const re::vec3 randomPoint() const override;

    // collision queries
    bool containsPoint(const re::vec3& point) const override;
    
    bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;
  
    re::PlaneQuery::FastResult fastPlaneIntersect(const re::vec3& normal, const re::vec3& center) const override;

  private:
    re::vec3 _normal;
    reFloat _offset;
  };

  /**
   * @ingroup shapes
   * Creates a plane from the given normal vector and an arbitrary
   * point on the plane.
   */

  inline Plane::Plane(const re::vec3& normal, const re::vec3& point) : _normal(normal), _offset(re::dot(normal, point)) {
    // do nothing
  }

  inline Plane::Plane(const re::vec3& normal, reFloat offset) : _normal(normal), _offset(offset) {
    // do nothing
  }

  inline reShape::Type Plane::type() const {
    return reShape::PLANE;
  }

  inline reUInt Plane::numVerts() const {
    return 0;
  }

  inline const re::vec3 Plane::vert(reUInt) const {
    return re::vec3();
  }

  inline reFloat Plane::shell() const {
    return RE_FP_TOLERANCE;
  }

  inline reFloat Plane::volume() const {
    return 0.0;
  }

  inline const re::mat3 Plane::computeInertia() const {
    RE_NOT_IMPLEMENTED
    return re::mat3(1.0);
  }

  inline const re::vec3 Plane::offset() const {
    return _offset * _normal;
  }

  inline const re::vec3 Plane::randomPoint() const {
    return _normal * _offset + re::cross(_normal, re::vec3::rand(1000.0));
  }
}

#endif
