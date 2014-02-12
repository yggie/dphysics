#ifndef RE_RAY_H
#define RE_RAY_H

#include "react/math.h"

namespace re {

  /**
   * @ingroup shapes
   * Defines an infinitely long segment
   */

  class Ray {
  public:
    Ray(const re::vec3& origin, const re::vec3& dir);

    const re::vec3& origin() const;
    const re::vec3& dir() const;

  private:
    re::vec3 _origin;
    re::vec3 _dir;
  };

  inline Ray::Ray(const re::vec3& origin, const re::vec3& dir) : _origin(origin), _dir(re::normalize(dir)) {
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
