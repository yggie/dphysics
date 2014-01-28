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
  
  const re::mat3 rot() const override;
  const re::quat& quat() const override;
  const re::vec3& angVel() const override;
  
  void setFacing(const re::vec3& dir, reFloat angle = 0.0) override;
  
protected:
  /** The reSolid's rotation matrix */
  re::quat _quat;
  /** The reSolid's angular velocity vector */
  re::vec _angVel;
};

inline reSolid::reSolid(reShape* shape) : reEnt(shape), _quat(), _angVel() {
  // do nothing
}

inline reSolid::~reSolid() {
  // do nothing
}

inline const re::mat3 reSolid::rot() const {
  return re::toMat(_quat);
}

inline const re::quat& reSolid::quat() const {
  return _quat;
}

inline const re::vec& reSolid::angVel() const {
  return _angVel;
}

#include "react/debug.h"

inline void reSolid::setFacing(const re::vec& dir, reFloat angle) {
  re::vec3 v = re::normalize(dir);
  re::vec3 up(0.0, 0.0, 1.0);
  if (reAbs(re::dot(v, up) - 1.0) < RE_FP_TOLERANCE) {
    up.set(0.0, 1.0, 0.0);
  }
  const re::vec u = re::cross(v, up);
  const re::vec w = re::cross(u, v);
  re::mat3 mm(
    u[0], v[0], w[0],
    u[1], v[1], w[1],
    u[2], v[2], w[2]
  );
  printf("%f\n\n", re::determinant(mm));
  _quat = re::toQuat(mm);
  rePrint(mm);
  printf("\n\n");
  rePrint(re::toMat(_quat));
  printf("\n\n");
  _quat *= re::axisAngleQ(re::vec(0, 1, 0), angle);
  rePrint(re::toMat(_quat));
  printf("\n\n");
}

#endif
