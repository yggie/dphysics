#ifndef DEMO_RAYTRACING_H
#define DEMO_RAYTRACING_H

#include "react/math.h"
#include "react/reWorld.h"

#include "demos/Common/glsetup.h"
#include "demos/Common/App.h"
#include "demos/RayTracing/RayLightSource.h"

#include <string>
#include <vector>

namespace re {
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
      
      re::demo::App::Options options() const override;
      
    private:
      const re::vec shootRay(unsigned int depth, const re::vec& origin, const re::vec& dir);
      void colorPixel(GLubyte* rgbaPixel, const re::vec& color);
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
      
      re::vec _ambient;
      re::vec _attenuation;
      
      std::vector<RayLightSource*> _lights;
      
      GLubyte* _pixels;
      
      GLsizei _renderWidth;
      GLsizei _renderHeight;
      
      re::vec _infinityColor;
      
      std::string _sceneFile;
      unsigned int _lightNo;
    };

    inline re::demo::App::Options RayTracingDemo::options() const {
      re::demo::App::Options opt;
      opt.trapMouse = false;
      opt.dynamic = false;
      return opt;
    }

    inline void RayTracingDemo::colorPixel(GLubyte* rgbaPixel, const re::vec& color) {
      for (int i = 0; i < 3; i++) {
        const float c = (color[i] > 1.0) ? 1.0 : color[i];
        rgbaPixel[i] = (GLubyte)(254.5 * ((c < 0.0) ? 0.0 : c));
        printf("%x", rgbaPixel[i]);
      }
      printf(":");
      rgbaPixel[3] = 0xff;
    }
  }
}

#endif
