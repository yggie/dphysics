#ifndef RAY_IMAGE_H
#define RAY_IMAGE_H

#include <GL/gl.h>

class RayImage {
public:
  RayImage();
  ~RayImage();
  
  void draw();
  void resize(GLuint w, GLuint h);
  
  GLubyte& red(GLuint i, GLuint j);
  GLubyte& blue(GLuint i, GLuint j);
  GLubyte& green(GLuint i, GLuint j);
  
private:
  GLubyte* _pixels;
  GLuint _w;
  GLuint _h;
};

inline void RayImage::draw() {
  glDrawPixels(_w, _h, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);
}

inline GLubyte& RayImage::red(GLuint i, GLuint j) {
  return _pixels[4*i*_h + 4*j + 0];
}

inline GLubyte& RayImage::blue(GLuint i, GLuint j) {
  return _pixels[4*i*_h + 4*j + 1];
}

inline GLubyte& RayImage::green(GLuint i, GLuint j) {
  return _pixels[4*i*_h + 4*j + 2];
}

#endif
