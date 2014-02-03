#ifndef DEMO_MATH_EXT_H
#define DEMO_MATH_EXT_H

#include "react/math.h"

namespace re {
  namespace demo {  
    inline const re::mat4x4 facing(const re::vec3& dir, const re::vec3& up) {
      const re::vec3 w = re::normalize(dir);
      const re::vec3 u = re::normalize(re::cross(up, w));
      const re::vec3 v = re::normalize(re::cross(w, u));
      
      return re::mat4x4(
		      u[0], u[1], u[2],  0,
		      v[0], v[1], v[2],  0,
		      w[0], w[1], w[2],  0,
		         0,    0,    0,  1
      );
    }
  }
}

#endif
