#ifndef DEMO_RAYTRACING_H
#define DEMO_RAYTRACING_H

#include "react/math.h"
#include "react/reWorld.h"

#include "demos/Common/glsetup.h"
#include "demos/Common/App.h"
#include "demos/RayTracing/RayLightSource.h"

#include <string>
#include <vector>

namespace demo {
  class RayTracingDemo : public App {
  public:
    RayTracingDemo();
    ~RayTracingDemo();
    
    void init() override;
    void restart() override;
    void release() override;
    void draw() override;
    void onResize(unsigned int w, unsigned int h) override;
    void keyEvent(unsigned char key, int x, int y) override;
    void specialKeyEvent(int key, int x, int y) override;
    void mouseEvent(int button, int state, int x, int y) override;
    
    App::Options options() const override;
    
  private:
    const re::vec3 shootRay(unsigned int depth, const re::vec3& origin, const re::vec3& dir);
    void colorPixel(GLubyte* rgbaPixel, const re::vec3& color);
    void resizeImage(GLsizei w, GLsizei h);
    void renderScene(GLsizei w, GLsizei h);
    void createSceneFromFile(const char* filename);
    
    reWorld _world;
    GLuint _maxDepth;
    GLsizei _imageWidth;
    GLsizei _imageHeight;
    std::string _outputFile;
    
    float _fovy;
    re::mat4x4 _viewMat;
    re::mat4x4 _inverseViewMat;
    
    re::vec3 _ambient;
    re::vec3 _attenuation;
    
    std::vector<RayLightSource*> _lights;
    
    GLubyte* _pixels;
    
    GLsizei _renderWidth;
    GLsizei _renderHeight;
    
    re::vec3 _infinityColor;
    
    std::string _sceneFile;
    unsigned int _lightNo;
    reUInt _raysSent;
  };

  inline App::Options RayTracingDemo::options() const {
    App::Options opt;
    opt.trapMouse = false;
    opt.dynamic = false;
    return opt;
  }

  inline void RayTracingDemo::colorPixel(GLubyte* rgbaPixel, const re::vec3& color) {
    for (int i = 0; i < 3; i++) {
      const float c = (color[i] > 1.0) ? 1.0 : color[i];
      rgbaPixel[i] = (GLubyte)(254.5 * ((c < 0.0) ? 0.0 : c));
      printf("%x", rgbaPixel[i]);
    }
    printf(":");
    rgbaPixel[3] = 0xff;
  }
}

#endif
