#ifndef DEMO_PLANE_H
#define DEMO_PLANE_H

#include "demos/Common/SceneObject.h"

namespace demo {
  
  class Plane : public SceneObject {
  public:
    Plane();
    ~Plane();
    
    Type type() const override;
    bool isDynamic() const override;
    
    void draw();
    void draw(Canvas&) override { }
    void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas) override;

    GLuint numVAOReq() const override;
    GLuint numVBOReq() const override;
    GLuint numTBOReq() const override;
    
    struct Instance : public ObjectInstance {
      Instance(const re::vec3& norm, const re::vec3& a, const re::vec3& cent, float w, float b) : normal(norm), side(a), center(cent), width(w), breath(b) { }
      
      void draw(Canvas& canvas) override;
      SceneObject::Type type() const override { return SceneObject::PLANE; }
      bool isDynamic() const override { return false; }
      
      const re::vec3 normal;
      const re::vec3 side;
      const re::vec3 center;
      const float width;
      const float breath;
      ShaderMaterial material;
    };
    
  private:
    static GLuint _globalVAO;
    static GLuint _globalVBO;
  };
  
  inline SceneObject::Type Plane::type() const {
    return SceneObject::PLANE;
  }
  
  inline bool Plane::isDynamic() const {
    return false;
  }
  
  inline GLuint Plane::numVAOReq() const {
    return 1;
  }
  
  inline GLuint Plane::numVBOReq() const {
    return 1;
  }
  
  inline GLuint Plane::numTBOReq() const {
    return 0;
  }
}

#endif
