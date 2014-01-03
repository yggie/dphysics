#ifndef DEMO_STATICGFX_H
#define DEMO_STATICGFX_H

#include "demo/GfxObj.h"

#include <vector>

namespace demo {
  
  /**
   * @ingroup demo
   * For any static graphic objects. By static, meaning the graphical
   * definition doesn't change, but other data, such as position can
   * change. At the moment, it does not support textures
   */
  
  class StaticGfx : public GfxObj {
  public:
    
    class VAODef {
    public:
      
      struct BufferObj {
        GLvoid* data;
        GLint attr;
        GLsizei size;
        GLenum type;
        GLsizei stride;
      };
      
      VAODef(GLenum drawMode, GLsizei numVerts);
      ~VAODef();
      
      template <typename T>
      StaticGfx::VAODef& withAttrib(GLint attr, GLuint byteSize, T* data, GLsizei stride);
      
      GLuint vao;
      GLenum drawMode;
      GLsizei numVerts;
      std::vector<BufferObj> buffers;
    };
    
    StaticGfx();
    ~StaticGfx();
    
    StaticGfx::VAODef& newVAO(GLenum drawMode, GLsizei numVerts);
    
    Type type() const override;
    bool isDynamic() const override;
    
    void draw(Canvas& canvas) override;
    void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) override;
    GLuint numVAOReq() const override;
    GLuint numVBOReq() const override;
    GLuint numTBOReq() const override;
    
  private:
    std::vector<VAODef*> _vaoDefs;
  };
  
  inline GfxObj::Type StaticGfx::type() const {
    return STATIC_GRAPHIC;
  }
  
  inline bool StaticGfx::isDynamic() const {
    return false;
  }
  
  inline GLuint StaticGfx::numVAOReq() const {
    return _vaoDefs.size();
  }
  
  inline GLuint StaticGfx::numVBOReq() const {
    return _vaoDefs.size();
  }
  
  inline GLuint StaticGfx::numTBOReq() const {
    return 0;
  }

  template <typename T>
  inline StaticGfx::VAODef& StaticGfx::VAODef::withAttrib(GLint attr, GLuint byteSize, T* data, GLsizei stride) {
    // do nothing
    return *this;
  }

  template <>
  inline StaticGfx::VAODef& StaticGfx::VAODef::withAttrib<float>(GLint attr, GLuint byteSize, float* data, GLsizei stride) {
    StaticGfx::VAODef::BufferObj obj;
    obj.data = data;
    obj.size = byteSize;
    obj.type = GL_FLOAT;
    obj.attr = attr;
    obj.stride = stride;
    buffers.push_back(obj);
    return *this;
  }
}

#endif
