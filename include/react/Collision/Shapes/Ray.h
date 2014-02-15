#ifndef RE_RAY_H
#define RE_RAY_H

#include "react/math.h"

class reEnt;

namespace re {

  /**
   * @ingroup shapes
   * Defines an infinitely long segment
   */

  class Ray {
  public:
    Ray(const re::vec3& origin, const re::vec3& dir);
    Ray(const re::Ray& ray);
    Ray(const re::Ray& ray, const re::Transform& transform);

    const re::vec3& origin() const;
    const re::vec3& dir() const;

  private:
    re::vec3 _origin;
    re::vec3 _dir;
  };

  inline Ray::Ray(const re::vec3& origin, const re::vec3& dir) : _origin(origin), _dir(re::normalize(dir)) {
    // do nothing
  }

  inline Ray::Ray(const re::Ray& ray) : _origin(ray._origin), _dir(ray._dir) {
    // do nothing
  }

  inline Ray::Ray(const re::Ray& ray, const re::Transform& transform) : _origin(transform.applyToPoint(ray._origin)), _dir(re::normalize(transform.applyToDir(_dir))) {
    // do nothing
  }

  inline const re::vec3& Ray::origin() const {
    return _origin;
  }

  inline const re::vec3& Ray::dir() const {
    return _dir;
  }
}

#endif
