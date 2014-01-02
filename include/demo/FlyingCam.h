#ifndef DEMO_FLYINGCAM_H
#define DEMO_FLYINGCAM_H

#include "react/vector.h"
#include "demo/mat4.h"

namespace demo {
  
  /**
   * @ingroup demo
   * Simulates the smooth motion of a flying camera
   */
  
  class FlyingCam {
  public:
    FlyingCam();
    
    void step();
    void reset();
    
    void pushForward(float dist);
    void pushBack(float dist);
    void pushLeft(float dist);
    void pushRight(float dist);
    
    void rotLeft(float rad);
    void rotRight(float rad);
    void rotUp(float rad);
    void rotDown(float rad);
    
    mat4 viewMat();
    
  private:
    float _dampingFactor;
    re::vec _targetPos;
    re::vec _currPos;
    re::vec _targetRot;
    re::vec _currRot;
  };
  
  inline FlyingCam::FlyingCam() : _dampingFactor(0.99), _targetPos(), _currPos(), _targetRot(), _currRot() {
    reset();
  }
  
  inline void FlyingCam::step() {
    _currPos = _currPos * _dampingFactor + _targetPos * (1 - _dampingFactor);
    _currRot = _currRot * _dampingFactor + _targetRot * (1 - _dampingFactor);
  }
  
  inline void FlyingCam::reset() {
    _currPos.set(0, 0, 0);
    _targetPos.set(0, 0, 0);
    _currRot.set(0, 0, 0);
    _targetRot.set(0, 0, 0);
  }
  
  inline void FlyingCam::pushForward(float dist) {
    _targetPos[2] += dist;
  }
  
  inline void FlyingCam::pushBack(float dist) {
    _targetPos[2] -= dist;
  }
  
  inline void FlyingCam::pushLeft(float dist) {
    _targetPos[0] -= dist;
  }
  
  inline void FlyingCam::pushRight(float dist) {
    _targetPos[0] += dist;
  }
  
  inline void FlyingCam::rotLeft(float rad) {
    _targetRot[1] -= rad;
  }
  
  inline void FlyingCam::rotRight(float rad) {
    _targetRot[1] += rad;
  }
  
  inline void FlyingCam::rotUp(float rad) {
    _targetRot[0] += rad;
  }
  
  inline void FlyingCam::rotDown(float rad) {
    _targetRot[0] -= rad;
  }
  
  inline mat4 FlyingCam::viewMat() {
    mat4 m;
    m.rotate(_currRot[0], 1, 0, 0);
    m.rotate(_currRot[1], 0, 1, 0);
    m.rotateZ(_currRot[2]);
    m.translate(_currPos[0], _currPos[1], _currPos[2]);
    return m;
  }
}

#endif
