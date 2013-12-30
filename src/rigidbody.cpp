#include "react/rigidbody.h"

using namespace re;

RigidBody::RigidBody() : _vPos(), _mRot(), _vVel(), _mRotVel(), _sMass(1.0),
_mInertia() {
  // do nothing
}

RigidBody::~RigidBody() {
  // do nothing
}
