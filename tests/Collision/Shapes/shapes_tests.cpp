#include "helpers.h"

// volume-less shape types
#include "Ray.h"
#include "Plane.h"
#include "PlaneShape.h"

// complex shape types
#include "Sphere.h"
#include "ProxyShape.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
