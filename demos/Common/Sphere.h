#ifndef DEMO_SPHERE_H
#define DEMO_SPHERE_H

#include "demos/Common/EntityWrapper.h"

#include "react/Entities/reEnt.h"
#include "react/Collision/Shapes/reSphere.h"

namespace re {
  
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
      
      const reSphere& shape() const;
      
      GLuint numVAOReq() const override;
      GLuint numVBOReq() const override;
      GLuint numTBOReq() const override;
      
      struct Wrapper : public EntityWrapper {
        Wrapper(const reEnt& ent);
        const reSphere& shape() const;
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
    
    inline Sphere::Wrapper::Wrapper(const reEnt& ent) : EntityWrapper(ent) {
      shape();
    }
    
    inline const reSphere& Sphere::Wrapper::shape() const {
      return *((reSphere*)entity.shape());
    }
  }
}

#endif
