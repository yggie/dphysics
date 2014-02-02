#include "helpers.h"

// math module unit tests
#include "vec3.h"
#include "mat3x3.h"
#include "quat.h"
#include "reTransform.h"
#include "mat4x4.h"
#include "reIntegrator.h"

// shape module unit tests
#include "reSphere.h"
#include "reProxyShape.h"

// entity module unit tests
#include "reRigidBody.h"

// collision module unit tests
#include "reBSPTree.h"

// integration tests
#include "defaultWorld.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
