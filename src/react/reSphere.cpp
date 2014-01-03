#include "react/reSphere.h"

reSphere::reSphere(reFloat radius) : reShape(), _sRadius(radius) {
  // do nothing
}

reSphere::reSphere(const reSphere& sphere) : reShape(), _sRadius(sphere._sRadius) {
  // do nothing
}

reSphere::~reSphere() {
  // do nothing
}

