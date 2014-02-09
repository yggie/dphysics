#ifndef RE_SEGMENT_H
#define RE_SEGMENT_H

#include "react/Collision/Shapes/reShape.h"

namespace re {

  /**
   * @ingroup shapes
   * Defines a line segment
   */

  class Segment : public reShape {
  public:
    Segment(const re::vec3& origin, const re::vec3& dir, reFloat length = RE_INFINITY);

    // shape representation
    Type type() const override;
    reUInt numVerts() const override;
    const re::vec3 vert(reUInt i) const override;
    reFloat shell() const override;

    // physical metrics
    reFloat volume() const override;
    const re::mat3 computeInertia() const override;
    const re::vec3 center() const override;

    // utility methods
    const re::vec3 randomPoint() const override;

    // collision queries
    bool containsPoint(const re::vec3& point) const override;
    bool intersectsRay(const reRayQuery& query, reRayQueryResult& result) const override;
    re::PlaneQuery::FastResult fastPlaneIntersect(const re::vec3& normal, const re::vec3& center) const override;

  private:
    re::vec3 _origin;
    re::vec3 _dir;
    reFloat _length;
  };

  inline Segment::Segment(const re::vec3& origin, const re::vec3& dir, reFloat length) : _origin(origin), _dir(re::normalize(dir)), _length(length) {
    // do nothing
  }

  inline reShape::Type Segment::type() const {
    return reShape::SEGMENT;
  }

  inline reUInt Segment::numVerts() const {
    return 2;
  }

  const re::vec3 Segment::vert(reUInt i) const {
    return (i == 0) ? _origin : _origin + _dir * _length;
  }

  inline reFloat Segment::shell() const {
    return RE_FP_TOLERANCE;
  }

  inline reFloat Segment::volume() const {
    return 0.0;
  }

  inline const re::mat3 Segment::computeInertia() const {
    return re::transpose(re::mat3(_dir, re::vec3(), re::vec3()));
  }

  inline const re::vec3 Segment::center() const {
    return _origin;
  }

  inline const re::vec3 Segment::randomPoint() const {
    return _origin + _dir * re::randf(0.0, _length);
  }
}

#endif
