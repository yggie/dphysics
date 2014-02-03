#ifndef DEMO_CANVAS_H
#define DEMO_CANVAS_H

#include "react/math.h"

#include "demos/Common/glsetup.h"
#include "demos/Common/MatrixStack.h"
#include "demos/Shaders/ShaderDetails.h"

#include <vector>

#define PROGRAM_ID_NOT_SET \
  printf("[DEMO]  Warning: Shader program ID not set\n");

#define INVALID_ATTRIBUTE_INDEX(who) \
  printf("[WARN]  Invalid shader attribute index for %s\n", who);

namespace re {
namespace demo {  
    class Shader;
    class SceneObject;
    
    /**
     * @ingroup demo
     * Manages a graphic context with underlying shaders
     */
    
    class Canvas {
    public:
      Canvas();
      virtual ~Canvas();
      
      void init();
      void use();
      virtual void release();
      void releaseObjects();
      
      void add(SceneObject* obj);
      void prepareScene();
      void renderScene();
      
      const ShaderAttributes& attrs() const;
      const ShaderUniforms& uniforms() const;
      
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
      std::vector<SceneObject*> _objects;
      
      // allocated VAO and VBO for the application
      GLuint* _VAOs;
      GLuint* _VBOs;
  //    GLuint* _TBOs;
      
      int _numVAO;
      int _numVBO;
  //    int _numTBO;
  
      bool _sceneReady;
      
      ShaderAttributes _attrs;
      ShaderUniforms _uniforms;
    };
    
    inline GLuint Canvas::programID() const {
      return _programID;
    }
    
    inline const ShaderAttributes& Canvas::attrs() const {
      return _attrs;
    }
    
    inline const ShaderUniforms& Canvas::uniforms() const {
      return _uniforms;
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
      glUniformMatrix4fv(uniforms().projMat(), 1, GL_TRUE, &_projMat[0][0]);
    }
  }
}

#endif
