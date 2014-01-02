#ifndef DEMO_GFXOBJ_H
#define DEMO_GFXOBJ_H

#include "demo/glsetup.h"
#include "react/Ent.h"

namespace demo {
  
  class App;
  class Canvas;
  
  /**
   * @ingroup demo
   * Represents a graphical entity which can be drawn, wraps around a
   * re::Ent. Each subclass specializes in drawing certain shapes
   * 
   * @see re::Ent
   */
  
  class GfxObj {
  public:
    
    enum Type {
      PLAIN_SPHERE
    };
    
    GfxObj(const re::Ent& ent);
    virtual ~GfxObj();
    
    virtual Type type() const = 0;
    virtual bool isDynamic() const = 0;
    
    virtual void draw(Canvas& canvas) = 0;
    virtual void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) = 0;
    virtual GLuint numVAOReq() const = 0;
    virtual GLuint numVBOReq() const = 0;
    virtual GLuint numTBOReq() const = 0;
    
    void setApp(App* app);
    
  protected:
    const re::Ent* _ent;
    App* _app;
  };
  
  inline GfxObj::GfxObj(const re::Ent& ent) : _ent(&ent), _app(nullptr) { }

  inline GfxObj::~GfxObj() { }
  
  inline void GfxObj::setApp(App* app) {
    _app = app;
  }
}

#endif
