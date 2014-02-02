#include "demos/Common/Shader.h"

#include <fstream>

using namespace re::demo;

Shader::Shader(const char* filename, int type) : _type(type), _id(0),
_src(), _filename(filename) {
  std::fstream f;
  f.open(filename, std::ios::in | std::ios::binary);
  if (f.is_open()) {
    f.seekg(0, std::ios::end);
    _src.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(&_src[0], _src.size());
    f.close();
  } else {
    printf("Error reading file %s\n", filename);
    throw 0;
  }
}

Shader::~Shader() {
  del();
  _src.clear();
}

bool Shader::compile() {
  if (_id != 0) {
    printf("[DEMO]  Shader \"%s\" is already initialized\n", _filename);
    return false;
  }
  
  _id = glCreateShader(_type);
  
  const char* source = src();
  glShaderSource(_id, 1, &source, NULL);
  
  glCompileShader(_id);
  if (isFalse(GL_COMPILE_STATUS)) {
    printLog();
    return false;
  }
  
  return true;
}

void Shader::del() {
  if (_id != 0) {
    glDeleteShader(_id);
    _id = 0;
  }
}

