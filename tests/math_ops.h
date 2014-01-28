#include "helpers.h"

using namespace re;

TEST(MathOpsTest, SkewMatrix) {
  const vec3 a = vec3::random();
  for (reUInt i = 0; i < NUM_REPEATS; i++) {
    const mat3x3 v = reVector::random();
    const mat3x3 sky = re::skew(v);
    
    ASSERT_VEC_EQ(re::cross(v, a), sky*v);
  }
}

