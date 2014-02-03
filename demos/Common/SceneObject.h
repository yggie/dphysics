#ifndef DEMO_SCENEOBJECT_H
#define DEMO_SCENEOBJECT_H

#include "demos/Shaders/ShaderDetails.h"

namespace re {
  namespace demo {
    class Canvas;
    
    /**
     * @ingroup demo
     * Represents a graphical entity which can be drawn
     */
    
    class SceneObject {
    public:
      
      enum Type {
        SIMPLE_SPHERE,
        STATIC_GRAPHIC
      };
      
      SceneObject();
      virtual ~SceneObject();
      
      virtual Type type() const = 0;
      virtual bool isDynamic() const = 0;
      
      virtual void draw(Canvas& canvas) = 0;
      virtual void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) = 0;
      virtual GLuint numVAOReq() const = 0;
      virtual GLuint numVBOReq() const = 0;
      virtual GLuint numTBOReq() const = 0;
      
      ShaderMaterial& material();
    
    protected:
      ShaderMaterial _material;
    };
    
    inline SceneObject::SceneObject() {
      // do nothing
    }

    inline SceneObject::~SceneObject() {
      // do nothing
    }
    
    inline ShaderMaterial& SceneObject::material() {
      return _material;
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
      SceneObject* obj;
    };
  }
}

#endif
