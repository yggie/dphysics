#ifndef DEMO_PlainSphere_H
#define DEMO_PlainSphere_H

#include "demo/GfxObj.h"
#include "demo/MatrixStack.h"
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
    void gDraw(MatrixStack& stack, const Program& program) override;
    void gSetup(GLint vao, GLint vbo, const MyProgram& program);
    
    const re::Sphere& shape() const;
    
    GLint numVAOReq() const override;
    GLint numVBOReq() const override;
    GLint numTBOReq() const override;
    
  private:
    static GLint _globalVAO;
    static GLint _globalVBO;
  };
  
  inline GfxObj::Type PlainSphere::type() const {
    return GfxObj::PLAIN_SPHERE;
  }
  
  inline bool PlainSphere::isDynamic() const {
    return false;
  }
  
  inline const re::Sphere& PlainSphere::shape() const {
    return *((re::Sphere*)_ent->shape());
  }
  
  inline GLint PlainSphere::numVAOReq() const {
    return 1;
  }
  
  inline GLint PlainSphere::numVBOReq() const {
    return 1;
  }
  
  inline GLint PlainSphere::numTBOReq() const {
    return 0;
  }
}

#endif
