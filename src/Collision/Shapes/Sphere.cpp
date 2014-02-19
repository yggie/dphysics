#include "react/Collision/Shapes/Sphere.h"

using namespace re;

Sphere::Sphere(reFloat radius) : reShape() {
  _shell = radius;
}

Sphere::Sphere(const Sphere& sphere) : reShape() {
  _shell = sphere.radius();
}

Sphere::~Sphere() {
  // do nothing
}

const re::vec3 Sphere::randomPoint() const {
  return re::normalize(re::vec3::rand()) * re::randf(0.0, 0.99) * radius();
}

bool Sphere::containsPoint(const re::vec3& point) const {
  return lengthSq(point) < radius()*radius();
}

