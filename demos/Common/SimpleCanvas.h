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
      
      const Canvas::GLAttributeIndex& attrs() const override;
      const Canvas::GLUniformIndex& uniforms() const override;
      
      void applyModelView() override;
      
    private:
      void postInit() override;
      
      Canvas::GLAttributeIndex _attrs;
      Canvas::GLUniformIndex _uniforms;
    };
    
    inline const Canvas::GLAttributeIndex& SimpleCanvas::attrs() const {
      return _attrs;
    }
    
    inline const Canvas::GLUniformIndex& SimpleCanvas::uniforms() const {
      return _uniforms;
    }
  }
}

#endif
