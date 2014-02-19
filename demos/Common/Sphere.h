#ifndef DEMO_SPHERE_H
#define DEMO_SPHERE_H

#include "demos/Common/EntityWrapper.h"

#include "react/Entities/Entity.h"
#include "react/Collision/Shapes/Sphere.h"

namespace demo {
  
  /**
   * @ingroup demo
   * Represents a single solid Sphere with pre-defined colors
   */
  
  class Sphere : public SceneObject {
  public:
    Sphere();
    ~Sphere();
    
    Type type() const override;
    bool isDynamic() const override;
    void draw();
    void draw(Canvas&) override { }
    void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas);
    
    GLuint numVAOReq() const override;
    GLuint numVBOReq() const override;
    GLuint numTBOReq() const override;
    
    struct Wrapper : public EntityWrapper {
      Wrapper(const re::Entity& ent);
      const re::Sphere& shape() const;
      void draw(Canvas& canvas) override;
    };
    
  private:
    static GLuint _globalVAO;
    static GLuint _globalVBO;
  };
  
  inline SceneObject::Type Sphere::type() const {
    return SceneObject::SPHERE;
  }
  
  inline bool Sphere::isDynamic() const {
    return false;
  }
  
  inline GLuint Sphere::numVAOReq() const {
    return 1;
  }
  
  inline GLuint Sphere::numVBOReq() const {
    return 1;
  }
  
  inline GLuint Sphere::numTBOReq() const {
    return 0;
  }
  
  inline Sphere::Wrapper::Wrapper(const re::Entity& ent) : EntityWrapper(ent) {
    shape();
  }
  
  inline const re::Sphere& Sphere::Wrapper::shape() const {
    return ((re::Sphere&)entity.shape());
  }
}

#endif
