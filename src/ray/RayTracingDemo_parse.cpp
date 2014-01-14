#include "ray/RayTracingDemo.h"

#include "react/Entities/reRigidBody.h"
#include "react/Collision/Shapes/shapes.h"
#include "ray/RayLightSource.h"
#include "ray/RayObject.h"
#include "demo/MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//#define RAY_PRINTF(...)   printf(__VA_ARGS__);
#define RAY_PRINTF(...)   

namespace {
  struct Pair {
    glm::vec3 vert;
    glm::vec3 norm;
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

GLenum RayTracingDemo::newOpenGLLight() {
  switch (_lightNo++) {
    case 0:
      return GL_LIGHT0;
    
    case 1:
      return GL_LIGHT1;
    
    case 2:
      return GL_LIGHT2;
    
    case 3:
      return GL_LIGHT3;
    
    case 4:
      return GL_LIGHT4;
      
    default:
      throw 0;
  }
}

/**
 * Parses the file and creates the scene
 */

void RayTracingDemo::createSceneFromFile(const char* filename, bool useOpenGL) {
  static bool toooogle = false;
  if (!useOpenGL) {
    release();
  } else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    _lightNo = 0;
    toooogle = !toooogle;
  }
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
  std::vector<glm::vec3> verts;
  std::vector<Pair> vertsWithNorms;
  
  // SPHERE DEFINITION
  unsigned int cSpheres = 0;
  
  // TRIANGLE DEFINITION
  unsigned int cTri = 0;
  unsigned int cTriNorm = 0;
  
  // MATERIALS DEFINITION
  glm::vec3 diffuse(0.0, 0.0, 0.0);
  glm::vec3 emission(0.0, 0.0, 0.0);
  glm::vec3 specular(0.0, 0.0, 0.0);
  float shininess = 0.0f;
  
  glm::mat4 view;
  
  // UNKNOWN COUNT
  unsigned int unknowns = 0;
  
  GLenum L;
  
  demo::MatrixStack stack;
  
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
      printf("    %-25s%4d, %4d   (%s)\n", "SIZE", s[0], s[1], line.c_str());
      
    } else if (cmd == "maxdepth") {
      readUInts(1, &_maxDepth);
      RAY_PRINTF("    %-30s%5d", "MAX DEPTH", _maxDepth)
      
    } else if (cmd == "output") {
      in >> _outputFile;
      printf("    %-6s%29s   (%s)\n", "OUTPUT", _outputFile.c_str(), line.c_str());
      
      /**
       * CAMERA RELATED COMMANDS
       */
      
    } else if (cmd == "camera") {
      float values[10];
      readFloats(10, &values[0]);
      
			glm::vec3 eye(values[0], values[1], values[2]);
			glm::vec3 center(values[3], values[4], values[5]);
			glm::vec3 up(values[6], values[7], values[8]);
			_fovy = values[9];

			glm::vec3 w = glm::normalize(eye - center);
			glm::vec3 u = glm::normalize(glm::cross(up, w));
			glm::vec3 v = glm::normalize(glm::cross(w, u));
			
			if (useOpenGL) {
			  glm::vec4 a(u, -glm::dot(eye, u));
			  glm::vec4 b(v, -glm::dot(eye, v));
			  glm::vec4 c(w, -glm::dot(eye, w));
			  glm::vec4 d(0, 0, 0, 1);
			  glm::mat4 view(a, b, c, d);
			  GLfloat near = 0.1;
			  GLfloat asRat = _imageWidth / (float)_imageHeight;
			  GLfloat y = glm::tan(_fovy/2.0) * near;
			  GLfloat x = asRat * y;
			  if (toooogle) {
			    glm::mat4 frus = glm::frustum(-x, x, -y, y, 0.1f, 100.0f);
			    glm::mat4 proj = (frus * glm::transpose(view));
  			  glMatrixMode(GL_PROJECTION);
  			  glLoadMatrixf(&proj[0][0]);
  			  glMatrixMode(GL_MODELVIEW);
			  } else {
			    glMatrixMode(GL_PROJECTION);
			    glLoadIdentity();
			    glm::mat4 m = glm::frustum(-x, x, -y, y, 0.1f, 100.0f);
			    glLoadMatrixf(&m[0][0]);
			    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
  			  glMatrixMode(GL_MODELVIEW);
			  }
			}
		  _viewMat = reTMatrix(
		    u[0], u[1], u[2], -glm::dot(eye, u),
		    v[0], v[1], v[2], -glm::dot(eye, v),
		    w[0], w[1], w[2], -glm::dot(eye, w),
		       0,    0,    0,         1
		  );
		  _inverseViewMat = _viewMat.inverse();
			RAY_PRINTF("    %-35s", "CAMERA")
      
      /**
       * GEOMETRY RELATED COMMANDS
       */
      
    } else if (cmd == "sphere") {
      float v[4];
      readFloats(4, &v[0]);
      
      if (useOpenGL) {
        glPushMatrix();
        glTranslatef(v[0], v[1], v[2]);
        glutSolidSphere(v[3], 20, 20);
        glPopMatrix();
      } else {
        glm::mat4 m = stack.mat();
        reTransform tm;
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            tm.m[i][j] = m[j][i];
          }
        }
        tm.v[0] = m[3][0];
        tm.v[1] = m[3][1];
        tm.v[2] = m[3][2];
        reEnt& ent = _world.newRigidBody().withShape(
          reDistortedShape(reSphere(v[3])).withDistortion(tm)
        ).at(v[0], v[1], v[2]);
        RayObject* obj = new RayObject();
        ent.userdata = obj;
        obj->withDiffuse(diffuse[0], diffuse[1], diffuse[2])
            .withSpecular(specular[0], specular[1], specular[2])
            .withEmission(emission[0], emission[1], emission[2])
            .withShininess(shininess);
      }
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
      glm::vec3 vert;
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
      p.vert = glm::vec3(a[0], a[1], a[2]);
      p.norm = glm::vec3(a[3], a[4], a[5]);
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
      glm::mat4 m = stack.mat();
      if (useOpenGL) {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
          glVertex3f(verts.at(inds[i])[0], verts.at(inds[i])[1], verts.at(inds[i])[2]);
        }
        glEnd();
      } else {
        glm::vec4 verts4[3];
        reVector triVerts[3];
        for (int i = 0; i < 3; i++) {
          verts4[i] = m * glm::vec4(verts.at(inds[i]), 1.0f);
          triVerts[i].set(verts4[i][0], verts4[i][1], verts4[i][2]);
        }
        reEnt& ent = _world.newRigidBody().withShape(
          reTriangle(triVerts[0], triVerts[1], triVerts[2])
        ).at(0, 0, 0);
        RayObject* obj = new RayObject();
        ent.userdata = obj;
        obj->withDiffuse(diffuse[0], diffuse[1], diffuse[2])
            .withSpecular(specular[0], specular[1], specular[2])
            .withEmission(emission[0], emission[1], emission[2])
            .withShininess(shininess);
      }
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
      if (useOpenGL) {
        glTranslatef(t[0], t[1], t[2]);
      } else {
        stack.translate(t[0], t[1], t[2]);
      }
      RAY_PRINTF("    %-12s%+11.1f, %+4.1f, %+4.1f",
                 "TRANSLATE", t[0], t[1], t[2])
      
    } else if (cmd == "rotate") {
      float r[4];
      readFloats(4, &r[0]);
      if (useOpenGL) {
        glRotatef(r[3], r[0], r[1], r[2]);
      } else {
        stack.rotate(r[3], r[0], r[1], r[2]);
      }
      RAY_PRINTF("    %-12s%6.1f %+4.1f, %+4.1f, %+4.1f",
                 "ROTATE", r[3], r[0], r[1], r[2])
      
    } else if (cmd == "scale") {
      float s[3];
      readFloats(3, &s[0]);
      if (useOpenGL) {
        glScalef(s[0], s[1], s[2]);
      } else {
        stack.scale(s[0], s[1], s[2]);
      }
      RAY_PRINTF("    %-19s%+4.1f, %+4.1f, %+4.1f",
                 "SCALE", s[0], s[1], s[2])
      
    } else if (cmd == "pushTransform") {
      if (useOpenGL) {
        glPushMatrix();
      } else {
        stack.push();
      }
      RAY_PRINTF("    %-32s%3d", "PUSH", stack.size())
      
    } else if (cmd == "popTransform") {
      if (useOpenGL) {
        glPopMatrix();
      } else {
        stack.pop();
      }
      RAY_PRINTF("    %-32s%3d", "POP", stack.size())
      
      /**
       * LIGHTING RELATED COMMANDS
       */

    } else if (cmd == "directional") {
      float a[6];
      readFloats(6, &a[0]);
      if (useOpenGL) {
        L = newOpenGLLight();
        float v[] = { a[3], a[4], a[5], 0.0 };
        glEnable(L);
        glLightfv(L, GL_POSITION, &v[0]);
        glLightfv(L, GL_DIFFUSE, &a[0]);
//        glLightfv(L, GL_POSITION, &a[3]);
      } else {
        RayLightSource* light = new RayLightSource();
        _lights.push_back(light);
        light->withColor(reVector(&a[3]))
              .withVect(reVector(&a[0]))
              .asDirectional(true);
      }
      
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "DIRECTIONAL LIGHT", a[3], a[4], a[5]);

    } else if (cmd == "point") {
      float a[6];
      readFloats(6, &a[0]);
      if (useOpenGL) {
        L = newOpenGLLight();
        float v[] = { a[3], a[4], a[5], 1.0 };
        glEnable(L);
        glLightfv(L, GL_POSITION, &v[0]);
        glLightfv(L, GL_DIFFUSE, &a[0]);
      } else {
        RayLightSource* light = new RayLightSource();
        _lights.push_back(light);
        light->withColor(reVector(&a[3]))
              .withVect(reVector(&a[0]))
              .asDirectional(false);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "SPOT LIGHT", a[3], a[4], a[5]);

    } else if (cmd == "attenuation") {
      float a[3];
      readFloats(3, &a[0]);
      if (useOpenGL) {
        glLightf(L, GL_CONSTANT_ATTENUATION, a[0]);
        glLightf(L, GL_LINEAR_ATTENUATION, a[1]);
        glLightf(L, GL_QUADRATIC_ATTENUATION, a[2]);
      } else {
        _attenuation.set(a[0], a[1], a[2]);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "ATTENUATION", a[0], a[1], a[2]);

    } else if (cmd == "ambient") {
      float a[3];
      readFloats(3, &a[0]);
      if (useOpenGL) {
        glLightfv(L, GL_AMBIENT, &a[0]);
      } else {
        _ambient.set(a[0], a[1], a[2]);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "AMBIENT", a[0], a[1], a[2]);

      /**
       * MATERIALS RELATED COMMANDS
       */
      
    } else if (cmd == "diffuse") {
      readFloats(3, &diffuse[0]);
      if (useOpenGL) {
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glColor3f(diffuse[0], diffuse[1], diffuse[2]);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse[0]);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "DIFFUSE", diffuse[0], diffuse[1], diffuse[2])
      
    } else if (cmd == "specular") {
      readFloats(3, &specular[0]);
      if (useOpenGL) {
        glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
        glColor3f(specular[0], specular[1], specular[2]);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular[0]);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "SPECULAR", specular[0], specular[1], specular[2])
      
    } else if (cmd == "emission") {
      readFloats(3, &emission[0]);
      if (useOpenGL) {
        glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
        glColor3f(emission[0], emission[1], emission[2]);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emission[0]);
      }
      RAY_PRINTF("    %-22s%3.1f, %3.1f, %3.1f", "EMISSION", emission[0], emission[1], emission[2])
      
    } else if (cmd == "shininess") {
      readFloats(1, &shininess);
      if (useOpenGL) {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
      }
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
  
  if (useOpenGL) {
    glFlush();
    for (int i= 0; i < 3; i++) {
      glPopMatrix();
    }
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);
    checkOpenGLError();
  } else {
    // precompute the transformed light vectors
    for (auto& light : _lights) {
      if (light->isDirectional()) {
        light->transformedVect() = _inverseViewMat.mult(light->vect(), 0.0);
      } else {
        light->transformedVect() = _inverseViewMat.mult(light->vect(), 1.0);
      }
    }
  }
}

