/**
 * @file
 * Contains the definition of the reBuilder class
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
}

/**
 * @ingroup utilities
 * Defines a builder class which provides factory methods for entities
 */

struct reBuilder {
  reBuilder(reWorld& world);
  
  // factory methods for entities
  reRigidBody& RigidBody(const reShape& shape);
  reRigidBody& RigidBody(const reShape& shape, const re::Transform& transform);
  
  // factory methods for interactions
  reGravAction& GravAction(reEnt& A, reEnt& B);
  
  // copy methods, uses internal allocator
  reShape* copyOf(const reShape& shape);
  
  reWorld& _world;
};

inline reBuilder::reBuilder(reWorld& world) : _world(world) {
  // do nothing
}

#endif
