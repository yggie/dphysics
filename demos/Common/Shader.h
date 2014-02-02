#ifndef DEMO_SHADER_H
#define DEMO_SHADER_H

#include "demos/Common/glsetup.h"

#include <string>
#include <cstdio>

namespace re {
  namespace demo {
  
    /**
     * @ingroup demo
     * Encapsulates an instance of a OpenGL shader program
     */
    
    class Shader {
    public:
      Shader(const char* filename, int type);
      ~Shader();
      
      int type() const;
      GLint id() const;
      const char* src() const;
      const char* filename() const;
      
      bool compile();
      void del();
      
      bool isFalse(GLenum option) const;
      void printLog() const;
      
    private:
      int _type;
      GLint _id;
      std::string _src;
      const char* _filename;
    };
    
    inline int Shader::type() const {
      return _type;
    }
    
    inline GLint Shader::id() const {
      return _id;
    }
    
    inline const char* Shader::src() const {
      return _src.c_str();
    }
    
    inline const char* Shader::filename() const {
      return _filename;
    }
    
    inline bool Shader::isFalse(GLenum option) const {
      GLint status;
      glGetShaderiv(_id, option, &status);
      return (status == GL_FALSE);
    }
    
    inline void Shader::printLog() const {
      GLint len;
      char buffer[LOG_BUFFER_LENGTH];
      glGetShaderInfoLog(_id, LOG_BUFFER_LENGTH, &len, &buffer[0]);
      printf("%s\n", &buffer[0]);
    }
  }
}

#endif
