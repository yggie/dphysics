#ifndef DEMO_GFXOBJ_H
#define DEMO_GFXOBJ_H

#include "demo/glsetup.h"

namespace demo {
  
  class App;
  class Canvas;
  
  /**
   * @ingroup demo
   * Represents a graphical entity which can be drawn
   * 
   * @see re::Ent
   */
  
  class GfxObj {
  public:
    
    enum Type {
      PLAIN_SPHERE,
      STATIC_GRAPHIC
    };
    
    GfxObj();
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
    App* _app;
  };
  
  inline GfxObj::GfxObj() : _app(nullptr) { }

  inline GfxObj::~GfxObj() { }
  
  inline void GfxObj::setApp(App* app) {
    _app = app;
  }
  
  /**
   * @ingroup demo
   * Used to internally manage VAO, VBO and TBO requests in function
   * App::gInit()
   */

  struct Request {
    Request() : vaoIndex(-1), vboIndex(-1), tboIndex(-1), obj(nullptr) { }
    int vaoIndex;
    int vboIndex;
    int tboIndex;
    GfxObj* obj;
  };
}

#endif
