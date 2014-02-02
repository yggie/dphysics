#include "demo/Canvas.h"

#include "demo/glsetup.h"
#include "demo/Shader.h"
#include "demo/GfxObj.h"

using namespace demo;

Canvas::Canvas() : _programID(0), _shaders(), _projMat(1.0f),
_viewMat(1.0f), _modelMatStack() {
}

Canvas::~Canvas() {
  release();
}

void Canvas::init() {
  if (_shaders.empty()) {
    return;
  }
  
  if (_programID != 0) {
    printf("[DEMO]  Shader program already created\n");
    return;
  }
  
  for (Shader* shader : _shaders) {
    if (!shader->compile()) {
      throw 0; // TODO proper error thrown
    }
  }
  
  _programID = glCreateProgram();
  for (Shader* shader : _shaders) {
    glAttachShader(_programID, shader->id());
  }
  
  glLinkProgram(_programID);
  if (isFalse(GL_LINK_STATUS)) {
    printLog();
    throw 0;
  }
  
  postInit();
  
  glUseProgram(_programID);
  
  prepareBuffers();
}
  
void Canvas::use() {
  if (_shaders.empty()) {
    return;
  }
  
  if (_programID != 0) {
    glUseProgram(_programID);
  } else {
    PROGRAM_ID_NOT_SET
  }
}

/**
 * Releases any resources used by the Canvas
 */

void Canvas::release() {
  releaseObjects();
  if (_shaders.empty()) {
    return;
  }
  
  GLint currProgID;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currProgID);
  if ((GLuint)currProgID == _programID) {
    glUseProgram(0);
  }
  
  for (Shader* shader : _shaders) {
    if (_programID != 0) {
      glDetachShader(_programID, shader->id());
    }
    delete shader;
  }
  _shaders.clear();
  
  if (_programID != 0) {
    glDeleteProgram(_programID);
  }
  
  _programID = 0;
}

void Canvas::releaseObjects() {
  if (_objects.empty()) {
    return;
  }
  
  for (GfxObj* obj : _objects) {
    delete obj;
  }
  _objects.clear();
  
//  glDeleteTextures(_numTBO, _TBOs);
  glDeleteVertexArrays(_numVAO, _VAOs);
  
//  delete[] _TBOs;
  delete[] _VAOs;
  delete[] _VBOs;
  
  _VAOs = nullptr;
  _VBOs = nullptr;
}

void Canvas::add(GfxObj* obj) {
  if (_programID != 0) {
    throw 0;
  }
  
  _objects.push_back(obj);
}

bool Canvas::isFalse(GLenum option) const {
  GLint status;
  glGetProgramiv(_programID, option, &status);
  return (status == GL_FALSE);
}

void Canvas::printLog() {
  if (_programID != 0) {
    GLint len;
    char buffer[LOG_BUFFER_LENGTH];
    glGetProgramInfoLog(_programID, LOG_BUFFER_LENGTH, &len, &buffer[0]);
    printf("%s\n", &buffer[0]);
  } else {
    PROGRAM_ID_NOT_SET
  }
}

#include <map>
template <typename K, typename V>
V GetWithDef(const  std::map <K,V> & m, const K & key, const V & defval ) {
   typename std::map<K,V>::const_iterator it = m.find( key );
   if ( it == m.end() ) {
      return defval;
   }
   else {
      return it->second;
   }
}

void Canvas::prepareBuffers() {
  if (_objects.empty()) {
    return;
  }
  
  // define temporary request objects
  std::vector<Request> requests;
  
  int numVAO(0), numVBO(0);//, numTBO(0);
  std::map<GfxObj::Type, bool> processed;
  for (GfxObj* obj : _objects) {
    if (!GetWithDef(processed, obj->type(), false)) {
//      numTBO += obj->numTBOReq();
      // set request object
      Request req;
      req.obj = obj;
      
      // set VAO index if requested
      if (obj->numVAOReq() > 0) {
        req.vaoIndex = numVAO;
        numVAO += obj->numVAOReq();
      }
      
      // set VBO index if requested
      if (obj->numVBOReq() > 0) {
        req.vboIndex = numVBO;
        numVBO += obj->numVBOReq();
      }
      
      requests.push_back(req);
      processed[obj->type()] = !obj->isDynamic();
    }
  }
  
  _numVAO = numVAO;
  _numVBO = numVBO;
//  _numTBO = numTBO;

  _VAOs = new GLuint[numVAO];
  _VBOs = new GLuint[numVBO];
//  _TBOs = new GLuint[numTBO];
  
  glGenBuffers(numVBO, _VBOs);
  glGenVertexArrays(numVAO, _VAOs);
//  glGenTextures(numTBO, _TBOs);
  
  for (Request& req : requests) {
    req.obj->setup(&_VAOs[req.vaoIndex], &_VBOs[req.vboIndex], *this);
  }
}

