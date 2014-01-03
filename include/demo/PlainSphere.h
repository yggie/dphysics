#ifndef DEMO_PlainSphere_H
#define DEMO_PlainSphere_H

#include "demo/GfxObj.h"
#include "demo/MatrixStack.h"
#include "react/Ent.h"
#include "react/Sphere.h"

namespace demo {
  
  /**
   * @ingroup demo
   * Represents a single solid PlainSphere with pre-defined colors
   */
  
  class PlainSphere : public GfxObj {
  public:
    PlainSphere(const re::Ent& ent);
    ~PlainSphere();
    
    Type type() const override;
    bool isDynamic() const override;
    void draw(Canvas& canvas) override;
    void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas);
    
    const re::Sphere& shape() const;
    
    GLuint numVAOReq() const override;
    GLuint numVBOReq() const override;
    GLuint numTBOReq() const override;
    
  private:
    const re::Ent& _ent;
    static GLuint _globalVAO;
    static GLuint _globalVBO;
  };
  
  inline GfxObj::Type PlainSphere::type() const {
    return GfxObj::PLAIN_SPHERE;
  }
  
  inline bool PlainSphere::isDynamic() const {
    return false;
  }
  
  inline const re::Sphere& PlainSphere::shape() const {
    return *((re::Sphere*)_ent.shape());
  }
  
  inline GLuint PlainSphere::numVAOReq() const {
    return 1;
  }
  
  inline GLuint PlainSphere::numVBOReq() const {
    return 1;
  }
  
  inline GLuint PlainSphere::numTBOReq() const {
    return 0;
  }
}

#endif
