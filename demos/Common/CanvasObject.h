#ifndef DEMO_CanvasObject_H
#define DEMO_CanvasObject_H

#include "glsetup.h"

namespace re {
  namespace demo {
    class Canvas;
    
    /**
     * @ingroup demo
     * Represents a graphical entity which can be drawn
     * 
     * @see re::Ent
     */
    
    class CanvasObject {
    public:
      
      enum Type {
        SIMPLE_SPHERE,
        STATIC_GRAPHIC
      };
      
      CanvasObject();
      virtual ~CanvasObject();
      
      virtual Type type() const = 0;
      virtual bool isDynamic() const = 0;
      
      virtual void draw(Canvas& canvas) = 0;
      virtual void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) = 0;
      virtual GLuint numVAOReq() const = 0;
      virtual GLuint numVBOReq() const = 0;
      virtual GLuint numTBOReq() const = 0;
    };
    
    inline CanvasObject::CanvasObject() {
      // do nothing
    }

    inline CanvasObject::~CanvasObject() {
      // do nothing
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
      CanvasObject* obj;
    };
  }
}

#endif
