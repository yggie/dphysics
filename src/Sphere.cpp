#include "react/Sphere.h"

using namespace re;

Sphere::Sphere(reFloat radius) : Shape(), _sRadius(radius) {
  // do nothing
}

Sphere::Sphere(const Sphere& sphere) : Shape(), _sRadius(sphere._sRadius) {
  // do nothing
}

Sphere::~Sphere() {
  // do nothing
}

