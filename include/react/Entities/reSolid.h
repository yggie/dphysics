/**
 * @file
 * Contains the definition for the reSolid abstract class
 */
#ifndef RE_SOLID_H
#define RE_SOLID_H

#include "react/Entities/reEnt.h"

/**
 * @ingroup entities
 * A type of reEnt which has a finite volume
 * 
 * @see reEnt
 */

class reSolid : public reEnt {
public:
  /** Default constructor does nothing */
  reSolid(reShape* shape);
  /** Default destructor does nothing */
  virtual ~reSolid();
  
  const reMatrix rot() const override;
  const reQuaternion& quat() const override;
  const reVector& angVel() const override;
  
  void setFacing(const reVector& dir, reFloat angle = 0.0) override;
  
protected:
  /** The reSolid's rotation matrix */
  reQuaternion _quat;
  /** The reSolid's angular velocity vector */
  reVector _angVel;
};

inline reSolid::reSolid(reShape* shape) : reEnt(shape), _quat(), _angVel() {
  // do nothing
}

inline reSolid::~reSolid() {
  // do nothing
}

inline const reMatrix reSolid::rot() const {
  return _quat.toMatrix();
}

inline const reQuaternion& reSolid::quat() const {
  return _quat;
}

inline const reVector& reSolid::angVel() const {
  return _angVel;
}

#include "react/debug.h"

inline void reSolid::setFacing(const reVector& dir, reFloat angle) {
  reVector v(dir);
  v.normalize();
  reVector up(0.0, 0.0, 1.0);
  if ((v - up).lengthSq() < RE_FP_TOLERANCE) {
    up.set(0.0, 1.0, 0.0);
  }
  const reVector u = up.cross(v);
  const reVector w = v.cross(u);
  reMatrix mm(
    u[0], v[0], w[0],
    u[1], v[1], w[1],
    u[2], v[2], w[2]
  );
  _quat.setFromMatrix(mm);
  rePrint(mm);
  if (reAbs(angle) > RE_FP_TOLERANCE) {
    _quat *= reQuaternion(angle, v);
  }
  rePrint(_quat.toMatrix());
}

#endif
