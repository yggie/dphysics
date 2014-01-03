#include "demo/StaticGfx.h"

#include "demo/Canvas.h"

#include <algorithm>

using namespace demo;

StaticGfx::StaticGfx() : _vaoDefs(){
  // do nothing
}

StaticGfx::~StaticGfx() {
  for_each(_vaoDefs.begin(), _vaoDefs.end(), [&](VAODef* def) {
    delete def;
  });
  _vaoDefs.clear();
}

StaticGfx::VAODef& StaticGfx::newVAO(GLenum drawMode, GLsizei numVerts) {
  StaticGfx::VAODef* def = new StaticGfx::VAODef(drawMode, numVerts);
  _vaoDefs.push_back(def);
  return *def;
}

void StaticGfx::draw(Canvas& canvas) {
  canvas.push();
  
  canvas.pop();
}

void StaticGfx::setup(GLuint* vao, GLuint* vbo, const Canvas&) {
  for (unsigned int i = 0; i < _vaoDefs.size(); i++) {
    VAODef& def = *_vaoDefs.at(i);
    def.vao = vao[i];
    GLuint totalSize = 0;
    for_each(def.buffers.begin(), def.buffers.end(), [&](StaticGfx::VAODef::BufferObj obj) {
      totalSize += obj.size;
    });
    
    // bind the vertex array object
    glBindVertexArray(vao[i]);
    checkOpenGLError();
    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    checkOpenGLError();
    // initialize storage
    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
    checkOpenGLError();
    
    // count the cumulative bytes used
    size_t offset = 0;
    for_each(def.buffers.begin(), def.buffers.end(), [&](StaticGfx::VAODef::BufferObj obj) {
      // TODO react to errors
      glBufferSubData(GL_ARRAY_BUFFER, offset, obj.size, obj.data);
      checkOpenGLError();
      glVertexAttribPointer(obj.attr, obj.stride, obj.type, GL_FALSE, 0, (GLvoid*)offset);
      checkOpenGLError();
      glEnableVertexAttribArray(obj.attr);
      checkOpenGLError();
      offset += obj.size;
    });
  }
}

StaticGfx::VAODef::VAODef(GLenum drawMode, GLsizei numVerts) : vao(0),
drawMode(drawMode), numVerts(numVerts), buffers() {
  // do nothing
}

StaticGfx::VAODef::~VAODef() {
  buffers.clear();
}

