#ifndef RE_SEGMENT_H
#define RE_SEGMENT_H

#include "react/math.h"

namespace re {

  /**
   * @ingroup shapes
   * Defines a line segment
   */

  struct Segment {
    Segment(const re::vec3& start, const re::vec3& end);

    re::vec3 start;
    re::vec3 end;
  };

  inline Segment::Segment(const re::vec3& s, const re::vec3& e) : start(s), end(e) {
    // do nothing
  }
}

#endif
