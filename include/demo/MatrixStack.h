#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#define GLM_FORCE_RADIANS

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef PI
#define PI    3.14159265359
#endif

#define MATRIXSTACK_DEFAULT_CAPACITY    5

#ifdef DEBUG
#include <cstdio>
inline void mprint(const glm::mat4& m) {
  for (int i = 0; i < 4; i++)
    printf(" | %+6.3f, %+6.3f, %+6.3f, %+6.3f |\n", m[i][0], m[i][1], m[i][2], m[i][3]);
}
#endif

/**
 * @ingroup demo
 * Defines a matrix stack
 */

class MatrixStack {
public:
  MatrixStack();
  MatrixStack(int initialCapacity);
  ~MatrixStack();     // inline

  int size() const;   // inline
  void clear();       // inline

  void push();
  void pop();             // inline
  glm::mat4& top();       // inline
  glm::mat4& get(int i);  // inline

  void scale(float s);                                    // inline
  void scale(float x, float y, float z);                  // inline
  void translate(float x, float y, float z);              // inline
  void rotate(float angle, float x, float y, float z);    // inline
  void loadIdentity();                                    // inline
  void loadMatrix(const glm::mat4& m);                    // inline
  
  void frustum(float left ,float right, float btm, float top, float near, float far); // inline

  const glm::mat4 mat();

private:
  void increaseHeap();

  /** the current position in the stack */
  int _head;
  /** the position in which the matrix has been cached */
  int _cacheHead;
  int _capacity;
  glm::mat4* _heap;
  glm::mat4 _cached;
};

inline MatrixStack::MatrixStack() : MatrixStack(MATRIXSTACK_DEFAULT_CAPACITY) { }

inline int MatrixStack::size() const {
  return (_head + 1);
}

inline void MatrixStack::clear() {
  _head = 0;
  _heap[0] = glm::mat4(1.0f);
}

inline void MatrixStack::push() {
  if (++_head >= _capacity) {
    increaseHeap();
  }
  _heap[_head] = glm::mat4(1.0f);
}

inline void MatrixStack::pop() {
  // force cache regeneration
  if (_cacheHead != -1) {
    _cacheHead = -1;
    _cached = glm::mat4(1.0f);
  }
  if (_head == 0) {
    _heap[0] = glm::mat4(1.0f);
  } else {
    _head--;
  }
}

inline glm::mat4x4& MatrixStack::top() {
  return get(_head);
}

inline glm::mat4& MatrixStack::get(int i) {
  return _heap[i];
}

inline void MatrixStack::scale(float s) {
  top() = glm::scale(top(), glm::vec3(s));
}

inline void MatrixStack::scale(float x, float y, float z) {
  top() = glm::scale(top(), glm::vec3(x, y, z));
}

inline void MatrixStack::translate(float x, float y, float z) {
  top() = glm::translate(top(), glm::vec3(x, y, z));
}

inline void MatrixStack::rotate(float angle, float x, float y, float z) {
  top() = glm::rotate(top(), float(PI) * angle / 180.0f, glm::vec3(x, y, z));
}

inline void MatrixStack::loadIdentity() {
  top() = glm::mat4(1.0f);
}

inline void MatrixStack::loadMatrix(const glm::mat4& m) {
  top() = m;
}

inline void MatrixStack::frustum(float left ,float right, float btm, float top, float near, float far) {
  this->top() = glm::frustum(left, right, btm, top, near, far);
}

#endif // MATRIXSTACK_H

