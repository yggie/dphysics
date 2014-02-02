#include "demos/Common/StaticObject.h"

#include "demos/Common/Canvas.h"

#include <algorithm>

using namespace re::demo;

StaticObject::StaticObject() : _vaoDefs(), _pos(0.0f) {
  // do nothing
}

StaticObject::~StaticObject() {
  for_each(_vaoDefs.begin(), _vaoDefs.end(), [&](VAODef* def) {
    delete def;
  });
  _vaoDefs.clear();
}

StaticObject::VAODef& StaticObject::newVAO(GLenum drawMode, GLsizei numVerts) {
  StaticObject::VAODef* def = new StaticObject::VAODef(drawMode, numVerts);
  _vaoDefs.push_back(def);
  return *def;
}

void StaticObject::draw(Canvas& canvas) {
  canvas.push();
  canvas.translate(_pos[0], _pos[1], _pos[2]);
  canvas.applyModelView();
  for_each(_vaoDefs.begin(), _vaoDefs.end(), [&](VAODef* def) {
    glBindVertexArray(def->vao);
    glBindBuffer(GL_ARRAY_BUFFER, def->vbo);
    glDrawArrays(def->drawMode, 0, def->numVerts);
  });
  canvas.pop();
}

void StaticObject::setup(GLuint* vao, GLuint* vbo, const Canvas&) {
  for (unsigned int i = 0; i < _vaoDefs.size(); i++) {
    VAODef& def = *_vaoDefs.at(i);
    def.vao = vao[i];
    def.vbo = vbo[i];
    GLuint totalSize = 0;
    for_each(def.buffers.begin(), def.buffers.end(), [&](const StaticObject::VAODef::BufferObj& obj) {
      totalSize += obj.size;
    });
    
    // bind the vertex array object
    glBindVertexArray(vao[i]);
    CHECK_GL_ERR;
    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
    CHECK_GL_ERR;
    // initialize storage
    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
    CHECK_GL_ERR;
    
    // count the cumulative bytes used
    size_t offset = 0;
    for_each(def.buffers.begin(), def.buffers.end(), [&](StaticObject::VAODef::BufferObj& obj) {
      if (obj.data != nullptr) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, obj.size, obj.data);
        CHECK_GL_ERR;
        glVertexAttribPointer(obj.attr, obj.stride, obj.type, GL_FALSE, 0, (GLvoid*)offset);
        CHECK_GL_ERR;
        glEnableVertexAttribArray(obj.attr);
        CHECK_GL_ERR;
        offset += obj.size;
        free(obj.data);
        obj.data = nullptr;
      }
    });
  }
}

StaticObject::VAODef::VAODef(GLenum drawMode, GLsizei numVerts) : vao(0),
vbo(0), drawMode(drawMode), numVerts(numVerts), buffers() {
  // do nothing
}

StaticObject::VAODef::~VAODef() {
  for_each(buffers.begin(), buffers.end(), [](BufferObj& obj) {
    if (obj.data != nullptr) {
      free(obj.data);
      obj.data = nullptr;
    }
  });
  buffers.clear();
}
