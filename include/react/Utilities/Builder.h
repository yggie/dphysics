/**
 * @file
 * Contains the definition of the Builder class
 */
#ifndef RE_BUILDER_H
#define RE_BUILDER_H

class reWorld;
class reShape;
class reGravAction;

namespace re {
  class Entity;
  class Rigid;
  class Transform;
  class Static;

  /**
   * @ingroup utilities
   * Defines a builder class which provides factory methods for 
   * creating entities
   */

  struct Builder {
    Builder(reWorld& world);
    
    // factory methods for entities
    re::Rigid& Rigid(const reShape& shape);
    re::Rigid& Rigid(const reShape& shape, const re::Transform& transform);
    re::Static& Static(const reShape& shape);
    re::Static& Static(const reShape& shape, const re::Transform& transform);
    
    // factory methods for interactions
    reGravAction& GravAction(Entity& A, Entity& B);

    // copy methods, uses internal allocator
    reShape* copyOf(const reShape& shape);

    reWorld& _world;
  };

  inline Builder::Builder(reWorld& world) : _world(world) {
    // do nothing
  }
}

#endif
