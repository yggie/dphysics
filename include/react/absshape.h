#ifndef RE_ABSSHAPE_H
#define RE_ABSSHAPE_H

#include "react/common.h"

namespace re {
  
  /**
   * Represents an abstract shape
   */
  
  class AbsShape {
  public:
    virtual ~AbsShape();
    
    /**
     * Computes the volume of the shape
     * 
     * @return The calculated volume in user-defined units
     */
    
    virtual reFloat volume() const = 0;
  };
  
  inline AbsShape::~AbsShape() { }
}

#endif
