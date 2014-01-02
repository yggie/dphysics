#ifndef DEMO_MYPROGRAM_H
#define DEMO_MYPROGRAM_H

#include "demo/Shader.h"
#include "demo/Program.h"
#include "demo/mat4.h"

#include <cstdio>

namespace demo {
  
  /**
   * @ingroup demo
   * A specialized Program class
   */
  
  class MyProgram : public Program {
  public:
    MyProgram();
    ~MyProgram();
    
    const Shader& vert() const;
    const Shader& frag() const;
    
    GLint modelMatAddr() const;
    GLint vertPosAddr() const;
    GLint vertColorAddr() const;

    void setModelMat(const mat4& model) const override;
    void setProjMat(const mat4& proj) const override;
    void setViewMat(const mat4& view) const override;
    
  private:
    void _init() override;
    
    GLint _vertPosAddr;
    GLint _vertColorAddr;
    
    GLint _modelMatAddr;
    GLint _viewMatAddr;
    GLint _projMatAddr;
  };
  
  inline const Shader& MyProgram::vert() const {
    return *_shaders.at(0);
  }
  
  inline const Shader& MyProgram::frag() const {
    return *_shaders.at(1);
  }
  
  inline GLint MyProgram::modelMatAddr() const {
    return _modelMatAddr;
  }
  
  inline GLint MyProgram::vertPosAddr() const {
    return _vertPosAddr;
  }
  
  inline GLint MyProgram::vertColorAddr() const {
    return _vertColorAddr;
  }
  
  inline void MyProgram::setModelMat(const mat4& model) const {
    if (_modelMatAddr != -1) {
      glUniformMatrix4fv(_modelMatAddr, 1, GL_TRUE, model.cptr());
    }
  }
  
  inline void MyProgram::setProjMat(const mat4& proj) const {
    if (_projMatAddr != -1) {
      glUniformMatrix4fv(_projMatAddr, 1, GL_TRUE, proj.cptr());
    }
  }
  
  inline void MyProgram::setViewMat(const mat4& view) const {
    if (_viewMatAddr != -1) {
      glUniformMatrix4fv(_viewMatAddr, 1, GL_TRUE, view.cptr());
    }
  }
}

#endif
