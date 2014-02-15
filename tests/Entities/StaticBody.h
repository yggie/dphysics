#include "helpers.h"

#include "react/Entities/StaticBody.h"

TEST(StaticBody, Constructor) {
  re::Sphere s(1.0);
  re::StaticBody body(&s);

  ASSERT_FLOAT_EQ(body.massInv(), 0.0) <<
    "should have inverse mass of zero";

  ASSERT_FLOAT_EQ(body.mass(), RE_INFINITY) <<
    "should have infinite mass";

  ASSERT_FLOAT_EQ(body.density(), RE_INFINITY) <<
    "should have infinite density";

  const re::mat3 m = body.inertiaInv();
  for (reUInt i = 0; i < 3; i++) {
    for (unsigned int j = 0; j < 3; j++) {
      ASSERT_FLOAT_EQ(m[i][j], 0.0) <<
        "should have all elements in the inertia inverse tensor set to zero";
    }
  }
}
