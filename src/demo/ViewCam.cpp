#include "demo/ViewCam.h"

#include "demo/glsetup.h"

using namespace demo;

namespace {
  const float step = 1.0f;
  const float ease = 0.007f;
}

ViewCam::ViewCam() : _view(), _facing(), _mode(FIRST_PERSON) {
  reset();
}

ViewCam::~ViewCam() {
  // do nothing
}

void ViewCam::init() {
  switch (_mode) {
    case FIRST_PERSON:
      glutSetCursor(GLUT_CURSOR_NONE);
      break;
    
    case SKY_VIEW:
      glutSetCursor(GLUT_CURSOR_INHERIT);
      break;
    
    case FOLLOWING:
      glutSetCursor(GLUT_CURSOR_NONE);
      break;
  }
}

void ViewCam::update() {
  _translation = (1.0 - ease) * _translation + ease * _target;
  _rota = (1.0 - ease) * _rota + ease * _rotTarget;
//  _facing = (1.0 - ease) * _facing + ease * _faceTarget;
//  _rot = re::facing(_facing, re::vec3(0.0, 1.0, 0.0));
//  _faceTarget = re::normalize(_faceTarget);
}

void ViewCam::reset() {
  _facing = re::vec3(0, 0, 1);
  _faceTarget = _facing;
  _view = re::facing4(_facing, re::vec3(0, 1, 0));
}
  
const re::mat4 ViewCam::viewMat() const {
  return re::mat4()
    .translate(-_translation)
    .rotate(_rota.y, 0, 1, 0)
    .rotate(_rota.x, 1, 0, 0);
}

#include "react/debug.h"

void ViewCam::keyEvent(unsigned char key, int, int) {
  re::mat3 R = re::transpose(re::mat3::rotation(_rota.y, 0, 1, 0) * re::mat3::rotation(_rota.x, 1, 0, 0));
  switch (key) {
    case 'w':
      _target += R * re::vec3(0.0, 0.0, -step);
      break;
    
    case 'a':
      _target += R * re::vec3(-step, 0.0, 0.0);
      break;
    
    case 's':
      _target += R * re::vec3(0.0, 0.0, step);
      break;
    
    case 'd':
      _target += R * re::vec3(step, 0.0, 0.0);
      break;
  }
}

void ViewCam::specialKeyEvent(int, int, int) {
  // TODO something smart
}

void ViewCam::mouseEvent(int button, int, int, int) {
  switch (button) {
    case 3: // wheel up?
      _view.translate(_facing * step);
      break;
    
    case 4: // wheel down?
      _view.translate(_facing * -step);
      break;
  }
}

void ViewCam::motionEvent(float dx, float dy) {
  if (re::abs(dx) > RE_FP_TOLERANCE && re::abs(dy) > RE_FP_TOLERANCE) {
  }
}

void ViewCam::passiveMotionEvent(float dx, float dy) {
  if (re::abs(dx) > RE_FP_TOLERANCE || re::abs(dy) > RE_FP_TOLERANCE) {
    _rotTarget.x += 10.0*dy;
    _rotTarget.y += 10.0*dx;
  }
}

