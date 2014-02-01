#include "helpers.h"

// math module unit tests
#include "mat3x3.h"
#include "vec3.h"
#include "quat.h"
#include "reIntegrator.h"

// shape module unit tests
#include "reSphere.h"
#include "reProxyShape.h"

// entity module unit tests
#include "reSolid.h"

// collision module tests
#include "reBSPTree.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
