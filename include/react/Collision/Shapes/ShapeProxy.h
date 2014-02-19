/**
 * @file
 * Contains the definition of the re::ShapeProxy class
 */
#ifndef RE_SHAPE_PROXY_H
#define RE_SHAPE_PROXY_H

#include "react/Collision/Shapes/reShape.h"

namespace re {

  /**
   * @ingroup shapes
   * A special wrapper class around an arbitrary shape which allows the
   * user to apply additional transformations on the shape, such as scaling
   * or rotating
   */

  class ShapeProxy : public reShape {
  public:
    ShapeProxy(reShape* shape);
    ShapeProxy(reShape* shape, const re::Transform& transform);
    ShapeProxy(const ShapeProxy&) = delete;
    ~ShapeProxy();
    
    ShapeProxy& operator=(const ShapeProxy&) = delete;
    
    reShape* shape();
    const reShape* shape() const;
    void setShape(const reShape& base);
    ShapeProxy& withShape(const reShape& base);
    
    const re::Transform& transform() const;
    void setTransform(const re::Transform& transform);
    ShapeProxy& withTransform(const re::Transform& transform);
    
    // shape representation
    reShape::Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;
    reFloat shell() const override;
    const re::vec3 center() const override;
    
    // physical metrics
    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    
    // utility methods
    const re::vec3 randomPoint() const override;
    
    // collision queries
    bool containsPoint(const re::vec3& point) const override;

  private:
    re::Transform _transform;
    reShape* const _shape;
  };

  inline void ShapeProxy::setTransform(const re::Transform& transform) {
    _transform = transform;
  }

  inline ShapeProxy& ShapeProxy::withTransform(const re::Transform& transform) {
    setTransform(transform);
    return *this;
  }

  inline reShape* ShapeProxy::shape() {
    return _shape;
  }

  inline const reShape* ShapeProxy::shape() const {
    return _shape;
  }

  inline const re::Transform& ShapeProxy::transform() const {
    return _transform;
  }

  inline reShape::Type ShapeProxy::type() const {
    return reShape::PROXY;
  }

  inline reUInt ShapeProxy::numVerts() const {
    if (_shape != nullptr) {
      return _shape->numVerts();
    } else {
      return 0;
    }
  }

  inline const re::vec3 ShapeProxy::vert(reUInt i) const {
    if (_shape != nullptr) {
      return _shape->vert(i);
    } else {
      return re::vec3(0.0, 0.0, 0.0);
    }
  }

  inline reFloat ShapeProxy::shell() const {
    if (_shape != nullptr) {
      return _shape->shell();
    } else {
      return reShape::shell();
    }
  }

  inline const re::vec3 ShapeProxy::center() const {
    if (_shape != nullptr) {
      return _shape->center() + _transform.v;
    } else {
      return _transform.v;
    }
  }

  inline reFloat ShapeProxy::volume() const {
  //  RE_NOT_IMPLEMENTED
    if (_shape != nullptr) {
      return _shape->volume();
    } else {
      return 0.0;
    }
  }

  inline const re::mat3 ShapeProxy::computeInertia() const {
  //  RE_NOT_IMPLEMENTED
    return re::mat3(1.0);
  }

  inline const re::vec3 ShapeProxy::randomPoint() const {
    return _transform.applyToPoint(_shape->randomPoint());
  }
}

#endif
