#ifndef DEMO_Sphere_H
#define DEMO_Sphere_H

#include "CanvasObject.h"
#include "MatrixStack.h"
#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/reSphere.h"

namespace re {
  
  namespace demo {
    
    /**
     * @ingroup demo
     * Represents a single solid Sphere with pre-defined colors
     */
    
    class Sphere : public CanvasObject {
    public:
      Sphere(const reEnt& ent);
      ~Sphere();
      
      Type type() const override;
      bool isDynamic() const override;
      void draw(Canvas& canvas) override;
      void setup(GLuint* vao, GLuint* vbo, const Canvas& canvas);
      
      const reSphere& shape() const;
      
      GLuint numVAOReq() const override;
      GLuint numVBOReq() const override;
      GLuint numTBOReq() const override;
      
    private:
      const reEnt& _ent;
      static GLuint _globalVAO;
      static GLuint _globalVBO;
    };
    
    inline CanvasObject::Type Sphere::type() const {
      return CanvasObject::SIMPLE_SPHERE;
    }
    
    inline bool Sphere::isDynamic() const {
      return false;
    }
    
    inline const reSphere& Sphere::shape() const {
      return *((reSphere*)_ent.shape());
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
  }
}

#endif
