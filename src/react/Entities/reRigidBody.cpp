#include "react/Entities/reRigidBody.h"

reRigidBody::reRigidBody(const reWorld* world) : reSolid(world), _vVel(), _mRotVel(), _sMass(1.0), _mInertia() {
  // do nothing
}

reRigidBody::~reRigidBody() {
  // do nothing
}
