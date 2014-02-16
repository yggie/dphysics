/**
 * @file
 * Contains the definition of the re::Plane class
 */
#ifndef RE_PLANE_SHAPE_H
#define RE_PLANE_SHAPE_H

#include "react/Collision/Shapes/reShape.h"

namespace re {

  /**
   * @ingroup shapes
   * Represents an infinite plane
   */

  class Plane : public reShape {
  public:
    Plane(const re::vec3& normal, const re::vec3& point);
    Plane(const re::vec3& normal, reFloat offset);
    Plane(const re::Plane& plane);
    Plane(const re::Plane& plane, const re::Transform& transform);

    Plane& operator=(const Plane& plane);

    Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;

    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    const re::vec3 center() const override;

    const re::vec3& normal() const;
    reFloat offset() const;

    const re::vec3 randomPoint() const;

    bool containsPoint(const re::vec3& point) const override;
  private:
    re::vec3 _normal;
    reFloat _offset;
  };

  inline Plane::Plane(const re::vec3& n, const re::vec3& point) : _normal(re::normalize(n)), _offset(re::dot(_normal, point)) {
    // do nothing
  }

  inline Plane::Plane(const re::vec3& n, reFloat o) : _normal(re::normalize(n)), _offset(o) {
    // do nothing
  }

  inline Plane::Plane(const Plane& plane) : _normal(plane._normal), _offset(plane._offset) {
    // do nothing
  }

  inline Plane::Plane(const Plane& plane, const re::Transform& transform) : _normal(re::normalize(transform.applyToDir(plane._normal))), _offset(re::dot(_normal, transform.applyToPoint(plane._normal * plane._offset))) {
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

  inline reFloat Plane::volume() const {
    return 0.0;
  }

  inline const re::mat3 Plane::computeInertia() const {
    return re::mat3(1.0);
  }

  inline const re::vec3 Plane::center() const {
    return normal() * offset();
  }

  inline const re::vec3& Plane::normal() const {
    return _normal;
  }

  inline reFloat Plane::offset() const {
    return _offset;
  }

  inline Plane& Plane::operator=(const Plane& plane) {
    _normal = plane._normal;
    _offset = plane._offset;
    return *this;
  }

  inline bool Plane::containsPoint(const re::vec3& point) const {
    return re::abs(re::dot(normal(), point) - offset()) < shell();
  }

  inline const re::vec3 Plane::randomPoint() const {
    return center() + re::randf(1000.0) * re::normalize(re::cross(normal(), re::vec3::rand()));
  }
}

#endif
