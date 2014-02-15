/**
 * @file
 * Contains definitions of utility classes to communicate with the shader
 */
#ifndef DEMO_SHADER_DETAILS_H
#define DEMO_SHADER_DETAILS_H

#include "react/math.h"
#include "demos/Common/glsetup.h"

namespace demo {
  struct ShaderVariable {
    ShaderVariable(const char* n) : index(-1), name(n) { }
    GLint index;
    const char* name;
  };
  
  struct ShaderAttributes {
    ShaderVariable vars[2] = {
      ShaderVariable("vPos"),
      ShaderVariable("vNorm")
    };
    
    const unsigned int size = sizeof(vars) / sizeof(ShaderVariable);
    
    GLint vertPos() const { return vars[0].index; }
    GLint vertNorm() const { return vars[1].index; }
  };
  
  struct ShaderUniforms {
    ShaderVariable vars[5] = {
      ShaderVariable("mModelView"),
      ShaderVariable("mProj"),
      ShaderVariable("mNorm"),
      ShaderVariable("alpha"),
      ShaderVariable("diffuse")
    };
    
    const unsigned int size = sizeof(vars) / sizeof(ShaderVariable);
    
    GLint modelViewMat() const { return vars[0].index; }
    GLint projMat() const { return vars[1].index; }
    GLint normMat() const { return vars[2].index; }
    GLint alpha() const { return vars[3].index; }
    GLint diffuse() const { return vars[4].index; }
  };
  
  struct ShaderMaterial {
    re::vec3 diffuse;
    GLfloat alpha = 1.0f;
    
    void apply(const ShaderUniforms& uniform) {
      glUniform1f(uniform.alpha(), alpha);
      glUniform3f(uniform.diffuse(), diffuse[0], diffuse[1], diffuse[2]);
    }
  };
}

#endif
