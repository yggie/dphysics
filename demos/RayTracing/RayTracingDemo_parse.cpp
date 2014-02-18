#include "RayTracingDemo.h"

#include "RayLightSource.h"
#include "RayObject.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"
#include "demos/Common/MatrixStack.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace demo;

//#define RAY_PRINTF(...)   printf(__VA_ARGS__);
#define RAY_PRINTF(...)   

namespace {
  struct Pair {
    re::vec3 vert;
    re::vec3 norm;
  };
  
  /**
   * Reads an array of values from the input
   */

  template <typename T>
  void readArray(std::string line, std::istringstream& ss,
                  int num, T* v) {
    for (int i = 0; i < num; i++) {
    ss >> v[i];
      if (ss.fail()) {
        printf("Failed to read %s for %d/%d value\n", line.c_str(), i, num);
        throw 0;
      }
    }
  }
};

/**
 * Parses the file and creates the scene
 */

void RayTracingDemo::createSceneFromFile(const char* filename) {
  release();
  std::ifstream file;
  file.open(filename, std::ifstream::in);
  
  if (!file.is_open()) {
    printf("[PARSE]  Failed to open file \"%s\"\n", filename);
    throw 0;
  }
  printf("[PARSE]  Creating scene from file \"%s\"\n", filename);
  
  // VERTEX DEFINITION
  unsigned int maxVerts = 0;
  unsigned int maxNorms = 0;
  unsigned int cVerts = 0;
  unsigned int cNorms = 0;
  std::vector<re::vec3> verts;
  std::vector<Pair> vertsWithNorms;
  
  // SPHERE DEFINITION
  unsigned int cSpheres = 0;
  
  // TRIANGLE DEFINITION
  unsigned int cTri = 0;
  unsigned int cTriNorm = 0;
  
  // MATERIALS DEFINITION
  re::vec3 diffuse(0.0, 0.0, 0.0);
  re::vec3 emission(0.0, 0.0, 0.0);
  re::vec3 specular(0.0, 0.0, 0.0);
  float shininess = 0.0f;
  
  re::mat4 view;
  
  // UNKNOWN COUNT
  unsigned int unknowns = 0;
  
  MatrixStack stack;
  
  for (std::string line; std::getline(file, line); ) {
    if (!(line.find_first_not_of(" \t\r\n") != std::string::npos) ||
        (line[0] == '#')) {
      continue;
    }
    
    std::istringstream in(line);
    
    auto readFloats = [&](int n, float* v) {
      readArray(line, in, n, v);
    };
    auto readUInts = [&](int n, unsigned int* v) {
      readArray(line, in, n, v);
    };
    
    std::string cmd;
    in >> cmd;
    
    /**
     * GENERAL COMMANDS
     */
    
    if (cmd == "size") {
      unsigned int s[2];
      readUInts(2, s);
      _imageWidth = s[0];
      _imageHeight = s[1];
      
    } else if (cmd == "maxdepth") {
      readUInts(1, &_maxDepth);
      RAY_PRINTF("    %-30s%5d", "MAX DEPTH", _maxDepth)
      
    } else if (cmd == "output") {
      in >> _outputFile;
      
      /**
       * CAMERA RELATED COMMANDS
       */
      
    } else if (cmd == "camera") {
      float values[10];
      readFloats(10, &values[0]);
      
			re::vec3 eye(values[0], values[1], values[2]);
			re::vec3 center(values[3], values[4], values[5]);
			re::vec3 up(values[6], values[7], values[8]);
			_fovy = values[9];
			
			_viewMat = re::lookat(eye, center, up);
		  _inverseViewMat = re::inverse(_viewMat);
			RAY_PRINTF("    %-35s", "CAMERA")
      
      /**
       * GEOMETRY RELATED COMMANDS
       */
      
    } else if (cmd == "sphere") {
      float v[4];
      readFloats(4, &v[0]);
      
      re::mat4 m = stack.mat();
      re::Transform tm = re::toTransform(m);
      reRigidBody& body = _world.build().RigidBody(re::Sphere(v[3]), tm).at(v[0], v[1], v[2]);
      RayObject* obj = new RayObject();
      body.userdata = obj;
//        printf("DIFFUSE: (%.2f, %.2f, %.2f)\n", specular[0], specular[1], specular[2]);
      obj->withDiffuse(diffuse[0], diffuse[1], diffuse[2])
          .withSpecular(specular[0], specular[1], specular[2])
          .withEmission(emission[0], emission[1], emission[2])
          .withShininess(shininess);
      RAY_PRINTF("    %-6s%6d%5.1fR %+4.1f, %+4.1f, %+4.1f",
                 "SPHERE", cSpheres, v[3], v[0], v[1], v[2])
      cSpheres++;

    } else if (cmd == "maxverts") {
      readUInts(1, &maxVerts);
      verts.clear();
      cVerts = 0;
      RAY_PRINTF("    %-30s%5d", "MAX VERTS", maxVerts)
      
    } else if (cmd == "maxvertnorms") {
      vertsWithNorms.clear();
      cNorms = 0;
      readUInts(1, &maxNorms);
      RAY_PRINTF("    %-30s%5d", "MAX NORMS", maxNorms)
      
    } else if (cmd == "vertex") {
      re::vec3 vert;
      readFloats(3, &vert[0]);
      verts.push_back(vert);
      RAY_PRINTF("    %-6s%6d%+11.1f, %+4.1f, %+4.1f",
                 "VERTEX", cVerts, vert[0], vert[1], vert[2])
      if (cVerts++ >= maxVerts) {
        printf("Exceeded max vertex!!\n");
        throw 0;
      }
      
    } else if (cmd == "vertexnormal") {
      float a[6];
      readFloats(3, &a[0]);
      Pair p;
      p.vert = re::vec3(a[0], a[1], a[2]);
      p.norm = re::vec3(a[3], a[4], a[5]);
      vertsWithNorms.push_back(p);
      RAY_PRINTF(
        "    %-6s#%-5d%+4.1f,%+4.1f,%+4.1f,%+2.0f,%+2.0f,%+2.0f",
        "NORMAL", cNorms, a[0], a[1], a[2], a[3], a[4], a[5])
      if (cNorms++ >= maxNorms) {
        printf("Exceeded max vertex with normals!!\n");
        throw 0;
      }
      
    } else if (cmd == "tri") {
      unsigned int inds[3];
      readUInts(3, &inds[0]);
      re::mat4 m = stack.mat();
      re::vec3 tverts[3];
      for (int i = 0; i < 3; i++) {
        tverts[i] = m.multPoint(verts.at(inds[i]));
      }
      reRigidBody& body = _world.build().RigidBody(
        reTriangle(tverts[0], tverts[1], tverts[2])
      ).at(0, 0, 0);
      RayObject* obj = new RayObject();
      body.userdata = obj;
      obj->withDiffuse(diffuse[0], diffuse[1], diffuse[2])
          .withSpecular(specular[0], specular[1], specular[2])
          .withEmission(emission[0], emission[1], emission[2])
          .withShininess(shininess);
      RAY_PRINTF("    %-6s%6d%13d,%4d,%4d",
                 "TRI", cTri++, inds[0], inds[1], inds[2])
      cTri++;
      
    } else if (cmd == "trinormal") {
      unsigned int inds[6];
      readUInts(6, &inds[0]);
      cTriNorm++;
      printf("YOU SHOULD NOT BE HERE!\n");
      throw 0;
      
      /**
       * TRANSFORMATION COMMANDS
       */
      
    } else if (cmd == "translate") {
      float t[3];
      readFloats(3, &t[0]);
      stack.translate(t[0], t[1], t[2]);
      RAY_PRINTF("    %-12s%+11.1f, %+4.1f, %+4.1f",
                 "TRANSLATE", t[0], t[1], t[2])
      
    } else if (cmd == "rotate") {
      float r[4];
      readFloats(4, &r[0]);
      stack.rotate(RE_PI * r[3] / 180.0, r[0], r[1], r[2]);
      RAY_PRINTF("    %-12s%6.1f %+4.1f, %+4.1f, %+4.1f",
                 "ROTATE", r[3], r[0], r[1], r[2])
      
    } else if (cmd == "scale") {
      float s[3];
      readFloats(3, &s[0]);
      stack.scale(s[0], s[1], s[2]);
      RAY_PRINTF("    %-19s%+4.1f, %+4.1f, %+4.1f",
                 "SCALE", s[0], s[1], s[2])
      
    } else if (cmd == "pushTransform") {
      stack.push();
      RAY_PRINTF("    %-32s%3d", "PUSH", stack.size())
      
    } else if (cmd == "popTransform") {
      stack.pop();
      RAY_PRINTF("    %-32s%3d", "POP", stack.size())
      
      /**
       * LIGHTING RELATED COMMANDS
       */

    } else if (cmd == "directional") {
      float a[6];
      readFloats(6, &a[0]);
      RayLightSource* light = new RayLightSource();
      _lights.push_back(light);
      light->withColor(re::vec3(&a[3]))
            .withVect(re::vec3(&a[0]))
            .asDirectional(true);
      
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "DIRECTIONAL LIGHT", a[3], a[4], a[5]);

    } else if (cmd == "point") {
      float a[6];
      readFloats(6, &a[0]);
      RayLightSource* light = new RayLightSource();
      _lights.push_back(light);
      light->withColor(re::vec3(&a[3]))
            .withVect(re::vec3(&a[0]))
            .asDirectional(false);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "SPOT LIGHT", a[3], a[4], a[5]);

    } else if (cmd == "attenuation") {
      float a[3];
      readFloats(3, &a[0]);
      _attenuation.set(a[0], a[1], a[2]);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "ATTENUATION", a[0], a[1], a[2]);

    } else if (cmd == "ambient") {
      float a[3];
      readFloats(3, &a[0]);
      _ambient.set(a[0], a[1], a[2]);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "AMBIENT", a[0], a[1], a[2]);

      /**
       * MATERIALS RELATED COMMANDS
       */
      
    } else if (cmd == "diffuse") {
      readFloats(3, &diffuse[0]);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "DIFFUSE", diffuse[0], diffuse[1], diffuse[2])
      
    } else if (cmd == "specular") {
      readFloats(3, &specular[0]);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "SPECULAR", specular[0], specular[1], specular[2])
      
    } else if (cmd == "emission") {
      readFloats(3, &emission[0]);
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "EMISSION", emission[0], emission[1], emission[2])
      
    } else if (cmd == "shininess") {
      readFloats(1, &shininess);
      RAY_PRINTF("    %-30s%5.1f", "SHININESS", shininess)
      
    } else {
      RAY_PRINTF("    %-35s", "!!UNKNOWN!!")
      unknowns++;
    }
    
    RAY_PRINTF("   (%s)\n", line.c_str())
  }
  
  printf("[PARSE]  End of file \"%s\"\n", filename);
  printf("[PARSE]  Found spheres(%d), tri(%d), trinormal(%d)\n", cSpheres, cTri, cTriNorm);
  if (unknowns > 0) {
    printf("[WARN]   Unknown count: %d\n", unknowns);
  }
  
  file.close();
  
  // precompute the transformed light vectors
  for (auto& light : _lights) {
    if (light->isDirectional()) {
      light->transformedVect() = _inverseViewMat.mult(light->vect(), 0.0);
    } else {
      light->transformedVect() = _inverseViewMat.mult(light->vect(), 1.0);
    }
  }
}

