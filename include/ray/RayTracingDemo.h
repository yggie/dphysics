#ifndef DEMO_RAYTRACING_H
#define DEMO_RAYTRACING_H

#include "react/math.h"
#include "react/reWorld.h"
#include "demo/glsetup.h"
#include "demo/DemoApp.h"
#include "ray/RayLightSource.h"

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

class RayTracingDemo : public DemoApp {
public:
  RayTracingDemo();
  ~RayTracingDemo();
  
  void init() override;
  void restart() override;
  void release() override;
  void draw() override;
  void onResize() override;
  void keyEvent(unsigned char key, int x, int y) override;
  void specialKeyEvent(int key, int x, int y) override;
  
private:
  reVector shootRay(unsigned int depth, const reVector& origin, const reVector& dir);
  void colorPixel(GLubyte* rgbaPixel, const reVector& color);
  void resizeImage(GLsizei w, GLsizei h);
  void renderScene(GLsizei w, GLsizei h);
  void createSceneFromFile(const char* filename, bool useOpenGL);
  GLenum newOpenGLLight();
  
  reWorld _world;
  GLuint _maxDepth;
  GLsizei _imageWidth;
  GLsizei _imageHeight;
  std::string _outputFile;
  
  float _fovy;
  reTMatrix _viewMat;
  reTMatrix _inverseViewMat;
  
  reVector _ambient;
  reVector _attenuation;
  
  std::vector<RayLightSource*> _lights;
  
  GLubyte* _pixels;
  
  GLsizei _renderWidth;
  GLsizei _renderHeight;
  
  reVector _infinityColor;
  
  std::string _sceneFile;
  unsigned int _lightNo;
  bool usingGL;
};

inline void RayTracingDemo::colorPixel(GLubyte* rgbaPixel, const reVector& color) {
  for (int i = 0; i < 3; i++) {
    float c = (color[i] > 1.0) ? 1.0 : color[i];
    rgbaPixel[i] = (GLubyte)(255 * ((c < 0.0) ? 0 : c));
  }
  rgbaPixel[3] = 0xff;
}

#endif
