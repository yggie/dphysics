#include "react/Utilities/reIntegrator.h"
#include <gtest/gtest.h>

#include <cstdio>

TEST(IntegratorTest, ConstantVelocity) {
  reVector p(0.0, 0.0, 0.0);
  reVector v(5.0, 1.0, 3.0);
  const reUInt steps = 1000;
  const reFloat dt = 1.0 / steps;
  
  reIntegrator ign;
  for (reUInt i = 0; i < steps; i++) {
    ign.integrate(p, v, dt);
  }
  
  for (reUInt i = 0; i < 3; i++) {
    EXPECT_LE(reAbs(v[i] - p[i]), 0.01);
  }
}

TEST(IntegratorTest, ConstantRotation) {
  const reUInt steps = 1000;
  const reFloat tr = 2.0;
  const reFloat ir = 1.0;
  const reVector ax(0.0, 0.0, 1.0);
  reVector w(0.0, 0.0, tr - ir);
  const reFloat dt = 1.0 / steps;
  reQuaternion q(ir, ax);
  reQuaternion p(tr, ax);
  
  reIntegrator ign;
  for (reUInt i = 0; i < steps; i++) {
    ign.integrate(q, w, dt);
  }
  
  for (reUInt i = 0; i < 4; i++) {
    EXPECT_LE(reAbs(q[i] - p[i]), 0.01);
  }
}

