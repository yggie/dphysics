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
  
  void setAngVel(const re::vec3& angularVelocity);
  void setAngVel(reFloat wx, reFloat wy, reFloat wz);
  void setFacing(const re::vec3& dir, const re::vec3& up = re::vec3(0.0, 0.0, 1.0)) override;
  
protected:
  /** The reSolid's rotation matrix */
  re::quat _quat;
  /** The reSolid's angular velocity vector */
  re::vec3 _angVel;
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

inline const re::vec3& reSolid::angVel() const {
  return _angVel;
}

inline void reSolid::setAngVel(const re::vec3& angVel) {
  _angVel = angVel;
}

inline void reSolid::setAngVel(reFloat wx, reFloat wy, reFloat wz) {
  _angVel.set(wx, wy, wz);
}

inline void reSolid::setFacing(const re::vec3& dir, const re::vec3& up) {
  re::vec3 v = re::normalize(dir);
  re::vec3 upn = re::normalize(up);
  if (reAbs(re::dot(v, upn) - 1.0) < RE_FP_TOLERANCE) {
    upn = re::normalize(re::vec3::random());
  }
  const re::vec3 u = re::normalize(re::cross(v, upn));
  const re::vec3 w = re::normalize(re::cross(u, v));
  re::mat3 mm(
    u[0], v[0], w[0],
    u[1], v[1], w[1],
    u[2], v[2], w[2]
  );
  _quat = re::toQuat(mm);
}

#endif
