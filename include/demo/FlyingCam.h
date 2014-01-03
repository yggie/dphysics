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
    
    const glm::mat4& viewMat() const;
    
  private:
    float _dampingFactor;
    glm::mat4 _viewMat;
    glm::vec3 _targetPos;
    glm::vec3 _targetRot;
  };
  
  inline FlyingCam::FlyingCam() : _dampingFactor(0.01), _viewMat(1.0f), _targetPos(0.0f), _targetRot(0.0f) {
    reset();
  }
  
  inline void FlyingCam::step() {
    const glm::vec3 dPos = _targetPos * _dampingFactor;
    const glm::vec3 dRot = _targetRot * _dampingFactor;
    _targetPos *= (1 - _dampingFactor);
    _targetRot *= (1 - _dampingFactor);
    glm::mat4 m = glm::translate(glm::mat4(1.0f), dPos);
    m = glm::rotate(m, dRot[0], glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, dRot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, dRot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    _viewMat = m * _viewMat;
  }
  
  inline void FlyingCam::reset() {
    _viewMat = glm::mat4(1.0f);
    _targetPos = glm::vec3(0.0f);
    _targetRot = glm::vec3(0.0f);
  }
  
  inline void FlyingCam::pushForward(float dist) {
    _targetPos[2] += dist;
  }
  
  inline void FlyingCam::pushBack(float dist) {
    _targetPos[2] -= dist;
  }
  
  inline void FlyingCam::pushLeft(float dist) {
    _targetPos[0] += dist;
  }
  
  inline void FlyingCam::pushRight(float dist) {
    _targetPos[0] -= dist;
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
  
  inline const glm::mat4& FlyingCam::viewMat() const {
    return _viewMat;
  }
}

#endif
