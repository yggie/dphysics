/**
 * @file
 * Contains the definition of the Builder class
 */
#ifndef RE_BUILDER_H
#define RE_BUILDER_H

class reWorld;
class reEnt;
class reRigidBody;
class reShape;
class reGravAction;

namespace re {
  class Transform;
  class StaticBody;

  /**
   * @ingroup utilities
   * Defines a builder class which provides factory methods for 
   * creating entities
   */

  struct Builder {
    Builder(reWorld& world);
    
    // factory methods for entities
    reRigidBody& RigidBody(const reShape& shape);
    reRigidBody& RigidBody(const reShape& shape, const re::Transform& transform);
    re::StaticBody& StaticBody(const reShape& shape);
    re::StaticBody& StaticBody(const reShape& shape, const re::Transform& transform);
    
    // factory methods for interactions
    reGravAction& GravAction(reEnt& A, reEnt& B);
    
    // copy methods, uses internal allocator
    reShape* copyOf(const reShape& shape);
    
    reWorld& _world;
  };

  inline Builder::Builder(reWorld& world) : _world(world) {
    // do nothing
  }
}

#endif
