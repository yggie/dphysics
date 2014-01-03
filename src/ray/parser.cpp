#include "ray/parser.h"

#include "ray/RayScene.h"

#define GLM_FORCE_RADIANS

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

#define RAY_PRINTF(...)   printf(__VA_ARGS__);

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

/**
 * Parses the file and creates the scene
 */

RayScene createSceneFromFile(const char* filename) {
  RayScene scene;
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
      scene.setSize(s[0], s[1]);
      RAY_PRINTF("    %-21s(%5d, %5d)", "SIZE", s[0], s[1])
      
    } else if (cmd == "maxdepth") {
      unsigned int depth;
      readUInts(1, &depth);
      scene.setMaxBounces(depth);
      RAY_PRINTF("    %-30s%5d", "MAX DEPTH", maxVerts)
      
    } else if (cmd == "output") {
      std::string filename;
      in >> filename;
      scene.setOutputFile(filename.c_str());
      RAY_PRINTF("    %-6s%29s", "OUTPUT", filename.c_str())
      
      /**
       * CAMERA RELATED COMMANDS
       */
      
    } else if (cmd == "camera") {
      float values[10];
      readFloats(10, &values[0]);
      
			glm::vec3 eye(values[0], values[1], values[2]);
			glm::vec3 center(values[3], values[4], values[5]);
			glm::vec3 up(values[6], values[7], values[8]);
			// TODO create projection matrix
			float fovy = values[9];

			float depth = glm::length(eye);
			glm::vec3 w = glm::normalize(eye - center);
			glm::vec3 u = glm::normalize(glm::cross(up, w));
			glm::vec3 v = glm::normalize(glm::cross(w, u));
			glm::vec4 a(u, 0);
			glm::vec4 b(v, 0);
			glm::vec4 c(w, -depth);
			glm::vec4 d(0, 0, 0, 1);
			
			// TODO confirm this
			glm::mat4 view = glm::translate(glm::mat4(a, b, c, d), -center);
			
			scene.setViewMat(view);
			scene.setFOVY(fovy);
			RAY_PRINTF("    %-35s", "CAMERA")
      
      /**
       * GEOMETRY RELATED COMMANDS
       */
      
    } else if (cmd == "sphere") {
      float v[4];
      readFloats(4, &v[0]);
      scene.addSphere(v[0], v[1], v[2], v[3]);
      RAY_PRINTF("    %-6s%6d%5.1fR %+4.1f, %+4.1f, %+4.1f",
                 "SPHERE", cSpheres++, v[3], v[0], v[1], v[2])

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
      
    } else {
      continue;
      RAY_PRINTF("    %-35s", "!!UNKNOWN!!")
    }
    
    RAY_PRINTF("   (%s)\n", line.c_str())
  }
  
  printf("[PARSE]  End of file \"%s\"\n", filename);
  
  file.close();
  
  return scene;
}

