#ifndef DEMO_FLYINGCAM_H
#define DEMO_FLYINGCAM_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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
    
    glm::mat4 viewMat();
    
  private:
    float _dampingFactor;
    glm::vec3 _targetPos;
    glm::vec3 _currPos;
    glm::vec3 _targetRot;
    glm::vec3 _currRot;
  };
  
  inline FlyingCam::FlyingCam() : _dampingFactor(0.99), _targetPos(), _currPos(), _targetRot(), _currRot() {
    reset();
  }
  
  inline void FlyingCam::step() {
    _currPos = _currPos * _dampingFactor + _targetPos * (1 - _dampingFactor);
    _currRot = _currRot * _dampingFactor + _targetRot * (1 - _dampingFactor);
  }
  
  inline void FlyingCam::reset() {
    _currPos = glm::vec3(0.0f);
    _targetPos = glm::vec3(0.0f);
    _currRot = glm::vec3(0.0f);
    _targetRot = glm::vec3(0.0f);
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
  
  inline glm::mat4 FlyingCam::viewMat() {
    glm::mat4 m(1.0f);
    m = glm::rotate(m, _currRot[0], glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, _currRot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, _currRot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    m = glm::translate(m, _currPos);
    return m;
  }
}

#endif
