#ifndef RE_PLANE_H
#define RE_PLANE_H

#include "react/math.h"

namespace re {

  /**
   * @ingroup shapes
   * Represents an infinite plane
   */

  class Plane {
  public:
    Plane(const re::vec3& normal, const re::vec3& point);
    Plane(const re::vec3& normal, reFloat offset);
    Plane(const re::Plane& plane);

    enum Location {
      FRONT_OF_PLANE,
      ON_PLANE,
      BACK_OF_PLANE
    };

    const re::vec3& normal() const;
    reFloat offset() const;

    Plane& operator=(const Plane& plane);

    static const Plane apply(const reTransform& transform, const Plane& plane);
  protected:
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

  inline const Plane Plane::apply(const reTransform& transform, const Plane& plane) {
    return Plane(transform.multDir(plane.normal()), transform.multPoint(plane.normal() * plane.offset()));
  }
}

#endif
