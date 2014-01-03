#ifndef DEMO_STATICGFX_H
#define DEMO_STATICGFX_H

#include "demo/GfxObj.h"

#include <glm/vec3.hpp>
#include <vector>
#include <cstring>
#include <cstdlib>

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
        BufferObj() : data(nullptr), attr(0), size(0), type(0), stride(0) { }
        GLvoid* data;
        GLint attr;
        GLsizei size;
        GLenum type;
        GLsizei stride;
      };
      
      VAODef(GLenum drawMode, GLsizei numVerts);
      ~VAODef();
      
      template <typename T>
      StaticGfx::VAODef& withAttrib(GLint attr, GLuint numBytes, T* data, GLsizei stride);
      
      GLuint vao;
      GLuint vbo;
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
    glm::vec3 _pos;
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
  inline StaticGfx::VAODef& StaticGfx::VAODef::withAttrib(GLint, GLuint, T*, GLsizei) {
    // do nothing
    printf("[DEMO]  Incompatible attribute type\n");
    return *this;
  }

  template <>
  inline StaticGfx::VAODef& StaticGfx::VAODef::withAttrib<float>(GLint attr, GLuint numBytes, float* data, GLsizei stride) {
    StaticGfx::VAODef::BufferObj obj;
    obj.data = malloc(numBytes);
    memcpy(obj.data, data, numBytes);
    obj.size = numBytes;
    obj.type = GL_FLOAT;
    obj.attr = attr;
    obj.stride = stride;
    buffers.push_back(obj);
    return *this;
  }

  template <>
  inline StaticGfx::VAODef& StaticGfx::VAODef::withAttrib<const float>(GLint attr, GLuint numBytes, const float* data, GLsizei stride) {
    StaticGfx::VAODef::BufferObj obj;
    obj.data = malloc(numBytes);
    memcpy(obj.data, data, numBytes);
    obj.size = numBytes;
    obj.type = GL_FLOAT;
    obj.attr = attr;
    obj.stride = stride;
    buffers.push_back(obj);
    return *this;
  }
}

#endif
