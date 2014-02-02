#ifndef DEMO_CANVAS_H
#define DEMO_CANVAS_H

#include "react/math.h"

#include "demos/Common/glsetup.h"
#include "demos/Common/MatrixStack.h"

#include <vector>

#define PROGRAM_ID_NOT_SET \
  printf("[DEMO]  Warning: Shader program ID not set\n");

#define INVALID_ATTRIBUTE_INDEX(who) \
  printf("[WARN]  Invalid shader attribute index for %s\n", who);

namespace re {
namespace demo {  
    class Shader;
    class CanvasObject;
    
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
      void use();
      virtual void release();
      void releaseObjects();
      
      void add(CanvasObject* obj);
      void prepareScene();
      void renderScene();
      
      virtual const GLAttributeIndex& attrs() const = 0;
      virtual const GLUniformIndex& uniforms() const = 0;
      
      GLuint programID() const;
      
      void push();            // inline
      void clearStack();      // inline
      void pop();             // inline
      virtual void applyModelView() = 0;
      
      const re::mat4& projMat() const;
      const re::mat4& viewMat() const;
      const re::mat4 modelMat();
      
      void setViewMat(const re::mat4& m);
      
      void scale(float s);                                    // inline
      void scale(float x, float y, float z);                  // inline
      void translate(float x, float y, float z);              // inline
      void rotate(float angle, float x, float y, float z);    // inline
      void loadIdentity();                                    // inline
      void loadMatrix(const re::mat4& m);                    // inline
      
      void frustum(float left ,float right, float btm, float top, float near, float far);
      
    protected:
      virtual void postInit() = 0;
      bool isFalse(GLenum option) const;
      void printLog();
      re::mat4 _mat();
      
      // shader programs
      GLuint _programID;
      std::vector<Shader*> _shaders;
      
      // shader uniforms
      re::mat4 _projMat;
      re::mat4 _viewMat;
      MatrixStack _modelMatStack;
      
      // graphic objects
      std::vector<CanvasObject*> _objects;
      
      // allocated VAO and VBO for the application
      GLuint* _VAOs;
      GLuint* _VBOs;
  //    GLuint* _TBOs;
      
      int _numVAO;
      int _numVBO;
  //    int _numTBO;
  
      bool _sceneReady;
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
    
    inline const re::mat4& Canvas::projMat() const {
      return _projMat;
    }
    
    inline const re::mat4& Canvas::viewMat() const {
      return _viewMat;
    }
    
    inline const re::mat4 Canvas::modelMat() {
      return _modelMatStack.mat();
    }
    
    inline void Canvas::setViewMat(const re::mat4& m) {
      _viewMat = m;
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

    inline void Canvas::loadMatrix(const re::mat4& m) {
      _modelMatStack.loadMatrix(m);
    }
    
    inline void Canvas::frustum(float left ,float right, float btm, float top, float near, float far) {
      _projMat = re::mat4x4(
        2.0*near/(right - left), 0.0, (right + left) / (right - left), 0.0,
        0.0, 2.0*near/(top - btm), (top + btm)/(top - btm), 0.0,
        0.0, 0.0, -(far + near)/(far - near), -2.0*far*near/(far - near),
        0.0, 0.0, -1.0, 0.0
      );
      if (uniforms().projMat) {
        glUniformMatrix4fv(uniforms().projMat, 1, GL_TRUE, &_projMat[0][0]);
      } else {
        INVALID_ATTRIBUTE_INDEX("mProj")
      }
    }
  }
}

#endif
