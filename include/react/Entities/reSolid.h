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

inline void reSolid::setFacing(const reVector& dir, reFloat angle) {
  reVector dr(dir);
  dr.normalize();
  reVector up(0.0, 0.0, 1.0);
  if ((dr - up).lengthSq() < RE_FP_TOLERANCE) {
    up.set(0.0, 1.0, 0.0);
  }
  const reVector nx = dr.cross(up);
  reMatrix mm(
    nx[0], nx[1], nx[2],
    dr[0], dr[1], dr[2],
    up[0], up[1], up[2]
  );
  for (reUInt i = 0; i < 3; i++) {
    printf(" | %.2f, %.2f, %.2f, |\n", mm[i][0], mm[i][1], mm[i][2]);
  }
  _quat.setFromMatrix(mm);
  _quat *= reQuaternion(angle, dr);
}

#endif
