#include "ray/RayImage.h"

RayImage::RayImage() : _pixels(nullptr), _w(0), _h(0) {
  resize(240, 180);
}

RayImage::~RayImage() {
  if (_pixels != nullptr) {
    delete[] _pixels;
  }
  _pixels = nullptr;
}

void RayImage::resize(GLuint w, GLuint h) {
  if (_pixels != nullptr) {
    delete[] _pixels;
  }
  
  const GLuint size = w * h * 4;
  _pixels = new GLubyte[size];
  for (GLuint i = 0; i < size; i++) {
    _pixels[i] = 0xFF;
  }
  
  _w = w;
  _h = h;
}
