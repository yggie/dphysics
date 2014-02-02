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
  _facing = (1.0 - ease) * _facing + ease * _faceTarget;
  _rot = re::facing(_facing, re::vec3(0.0, 1.0, 0.0));
  _faceTarget = re::normalize(_faceTarget);
}

void ViewCam::reset() {
  _facing = re::vec3(0, 0, -1);
  _faceTarget = _facing;
  _view = re::facing4(_facing, re::vec3(0, 1, 0));
}

void ViewCam::keyEvent(unsigned char key, int, int) {
  switch (key) {
    case 'w':
      _target += _rot * re::vec3(0.0, 0.0, step);
      break;
    
    case 'a':
      _target += _rot * re::vec3(step, 0.0, 0.0);
      break;
    
    case 's':
      _target += _rot * re::vec3(0.0, 0.0, -step);
      break;
    
    case 'd':
      _target += _rot * re::vec3(-step, 0.0, 0.0);
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
    _faceTarget.x += dx * 2.0;
    _faceTarget.y += dy * 2.0;
  }
}

