#ifndef DEMO_PROGRAM_H
#define DEMO_PROGRAM_H

#include "demo/glsetup.h"
#include "demo/mat4.h"

#include <string>
#include <cstdio>
#include <vector>

namespace demo {
  
  class Shader;
  
  /**
   * @ingroup demo
   * An encapsulates an instance of an OpenGL program
   */
  
  class Program {
  public:
    Program();
    virtual ~Program();
    
    void init();
    void del();
    
    GLuint id() const;
    
    bool isFalse(GLenum option) const;
    void printLog();
    
    virtual void setModelMat(const mat4& model) const = 0;
    virtual void setProjMat(const mat4& proj) const = 0;
    virtual void setViewMat(const mat4& view) const = 0;
    
  protected:
    virtual void _init() = 0;
    
    GLuint _id;
    std::vector<Shader*> _shaders;
  };
  
  inline GLuint Program::id() const {
    return _id;
  }
  
  inline bool Program::isFalse(GLenum option) const {
    GLint status;
    glGetProgramiv(_id, option, &status);
    return (status == GL_FALSE);
  }
  
  inline void Program::printLog() {
    if (_id != 0) {
      GLint len;
      char buffer[LOG_BUFFER_LENGTH];
      glGetProgramInfoLog(_id, LOG_BUFFER_LENGTH, &len, &buffer[0]);
      printf("%s\n", &buffer[0]);
    }
  }
}

#endif
