#ifndef DEMO_CANVAS_H
#define DEMO_CANVAS_H

#include "react/math.h"
#include "demo/glsetup.h"
#include "demo/MatrixStack.h"

#include <vector>

#define PROGRAM_ID_NOT_SET \
  printf("[DEMO]  Warning: Shader program ID not set\n");

namespace demo {
  
  class Shader;
  
  /**
   * @ingroup demo
   * Manages a graphic context with underlying shaders
   */
  
  class Canvas {
  public:
    
    struct GLAttributeIndex {
      GLAttributeIndex() : vertPos(-1), vertColor(-1), vertNorm(-1) { }
      GLint vertPos;
      GLint vertColor;
      GLint vertNorm;
    };
    
    struct GLUniformIndex {
      GLUniformIndex() : modelViewMat(-1), projMat(-1), normMat(-1) { }
      GLint modelViewMat;
      GLint projMat;
      GLint normMat;
    };
    
    Canvas();
    virtual ~Canvas();
    
    void init();
    void use();     // inline
    virtual void release();
    
    virtual const GLAttributeIndex& attrs() const = 0;
    virtual const GLUniformIndex& uniforms() const = 0;
    
    GLuint programID() const;
    
    void push();            // inline
    void clearStack();      // inline
    void pop();             // inline
    virtual void applyModelView() = 0;
    
    const glm::mat4& projMat() const;
    const glm::mat4& viewMat() const;
    const glm::mat4 modelMat();
    
    void setViewMat(const glm::mat4& m);
    void setViewMat(const re::mat4& m);
    
    void scale(float s);                                    // inline
    void scale(float x, float y, float z);                  // inline
    void translate(float x, float y, float z);              // inline
    void rotate(float angle, float x, float y, float z);    // inline
    void loadIdentity();                                    // inline
    void loadMatrix(const glm::mat4& m);                    // inline
    
    virtual void frustum(float left ,float right, float btm, float top, float near, float far) = 0;
    
  protected:
    virtual void postInit() = 0;
    bool isFalse(GLenum option) const;
    void printLog();
    glm::mat4 _mat();
    
    GLuint _programID;
    std::vector<Shader*> _shaders;
    
    glm::mat4 _projMat;
    glm::mat4 _viewMat;
    MatrixStack _modelMatStack;
  };
  
  inline GLuint Canvas::programID() const {
    return _programID;
  }
  
  inline void Canvas::push() {
    _modelMatStack.push();
  }
  
  inline void Canvas::clearStack() {
    _modelMatStack.clear();
  }
  
  inline void Canvas::pop() {
    _modelMatStack.pop();
  }
  
  inline const glm::mat4& Canvas::projMat() const {
    return _projMat;
  }
  
  inline const glm::mat4& Canvas::viewMat() const {
    return _viewMat;
  }
  
  inline const glm::mat4 Canvas::modelMat() {
    return _modelMatStack.mat();
  }
  
  inline void Canvas::setViewMat(const glm::mat4& m) {
    _viewMat = m;
  }
  
  inline void Canvas::setViewMat(const re::mat4& m) {
    _viewMat = glm::transpose(glm::mat4(
      m[0][0], m[0][1], m[0][2], m[0][3],
      m[1][0], m[1][1], m[1][2], m[1][3],
      m[2][0], m[2][1], m[2][2], m[2][3],
      m[3][0], m[3][1], m[3][2], m[3][3]
    ));
  }

  inline void Canvas::scale(float s) {
    _modelMatStack.scale(s);
  }

  inline void Canvas::scale(float x, float y, float z) {
    _modelMatStack.scale(x, y, z);
  }

  inline void Canvas::translate(float x, float y, float z) {
    _modelMatStack.translate(x, y, z);
  }

  inline void Canvas::rotate(float angle, float x, float y, float z) {
    _modelMatStack.rotate(angle, x, y, z);
  }

  inline void Canvas::loadIdentity() {
    _modelMatStack.loadIdentity();
  }

  inline void Canvas::loadMatrix(const glm::mat4& m) {
    _modelMatStack.loadMatrix(m);
  }
}

#endif
