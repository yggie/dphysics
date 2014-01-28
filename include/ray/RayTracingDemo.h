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
  void mouseEvent(int button, int state, int x, int y) override;
  
private:
  const re::vec shootRay(unsigned int depth, const re::vec& origin, const re::vec& dir);
  void colorPixel(GLubyte* rgbaPixel, const re::vec& color);
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
  
  re::vec _ambient;
  re::vec _attenuation;
  
  std::vector<RayLightSource*> _lights;
  
  GLubyte* _pixels;
  
  GLsizei _renderWidth;
  GLsizei _renderHeight;
  
  re::vec _infinityColor;
  
  std::string _sceneFile;
  unsigned int _lightNo;
  bool usingGL;
};

inline void RayTracingDemo::colorPixel(GLubyte* rgbaPixel, const re::vec& color) {
  for (int i = 0; i < 3; i++) {
    const float c = (color[i] > 1.0) ? 1.0 : color[i];
    rgbaPixel[i] = (GLubyte)(254.5 * ((c < 0.0) ? 0.0 : c));
    printf("%x", rgbaPixel[i]);
  }
  printf(":");
  rgbaPixel[3] = 0xff;
}

#endif
