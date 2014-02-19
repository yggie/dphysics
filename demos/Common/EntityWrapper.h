#ifndef ENTITY_WRAPPER_H
#define ENTITY_WRAPPER_H

#include "demos/Common/SceneObject.h"

namespace re {
  class Entity;
}

namespace demo {
  struct EntityWrapper : public SceneObject {
    EntityWrapper(const re::Entity& ent) : entity(ent) { }
    
    Type type() const { return SceneObject::SIMPLE_WRAPPER; }
    bool isDynamic() const { return false; }
    
    void setup(GLuint*, GLuint*, const Canvas&) { } // stub
    
    GLuint numVAOReq() const { return 0; }
    GLuint numVBOReq() const { return 0; }
    GLuint numTBOReq() const { return 0; }
    const re::Entity& entity;
    
    EntityWrapper& withColor(const re::vec3& c) { material.diffuse = c; return *this; }
    EntityWrapper& withAlpha(float alpha) { material.alpha = alpha; return *this; }
    
    ShaderMaterial material;
  };
}

#endif
