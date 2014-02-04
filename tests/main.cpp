#include "helpers.h"

// entity module unit tests
#include "reRigidBody.h"

//// utility module unit tests
//#include "reEntList.h"

// collision module unit tests
#include "reBSPTree.h"

// integration tests
#include "defaultWorld.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
