#ifndef DEMO_SIMPLECANVAS_H
#define DEMO_SIMPLECANVAS_H

#include "demos/Common/Canvas.h"

namespace re {  
  namespace demo {
    
    /**
     * @ingroup demo
     * A specialized Canvas class
     */
    
    class SimpleCanvas : public Canvas {
    public:
      SimpleCanvas();
      ~SimpleCanvas();
      
      void applyModelView() override;
      
    private:
      void postInit() override;
    };
  }
}

#endif
