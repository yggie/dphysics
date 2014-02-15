#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "react/math.h"

#define MATRIXSTACK_DEFAULT_CAPACITY    5

namespace demo {
  
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
    re::mat4& top();       // inline
    re::mat4& get(int i);  // inline

    void scale(float s);                                    // inline
    void scale(float x, float y, float z);                  // inline
    void translate(float x, float y, float z);              // inline
    void rotate(float angle, float x, float y, float z);    // inline
    void loadIdentity();                                    // inline
    void loadMatrix(const re::mat4& m);                    // inline
    
//      void frustum(float left ,float right, float btm, float top, float near, float far); // inline

    const re::mat4 mat();

  private:
    void increaseHeap();

    /** the current position in the stack */
    int _head;
    /** the position in which the matrix has been cached */
    int _cacheHead;
    int _capacity;
    re::mat4* _heap;
    re::mat4 _cached;
  };

  inline MatrixStack::MatrixStack() : MatrixStack(MATRIXSTACK_DEFAULT_CAPACITY) { }

  inline int MatrixStack::size() const {
    return (_head + 1);
  }

  inline void MatrixStack::clear() {
    _head = 0;
    _heap[0] = re::mat4(1.0f);
  }

  inline void MatrixStack::push() {
    if (++_head >= _capacity) {
      increaseHeap();
    }
    _heap[_head] = re::mat4(1.0f);
  }

  inline void MatrixStack::pop() {
    // force cache regeneration
    if (_cacheHead != -1) {
      _cacheHead = -1;
      _cached = re::mat4(1.0f);
    }
    if (_head == 0) {
      _heap[0] = re::mat4(1.0f);
    } else {
      _head--;
    }
  }

  inline re::mat4x4& MatrixStack::top() {
    return get(_head);
  }

  inline re::mat4& MatrixStack::get(int i) {
    return _heap[i];
  }

  inline void MatrixStack::scale(float s) {
    scale(s, s, s);
  }

  inline void MatrixStack::scale(float x, float y, float z) {
    top() *= re::mat4::scaling(x, y, z);
  }

  inline void MatrixStack::translate(float x, float y, float z) {
    top() *= re::mat4::translation(x, y, z);
  }

  inline void MatrixStack::rotate(float angle, float x, float y, float z) {
    top() *= re::mat4::rotation(angle, re::vec3(x, y, z));
  }

  inline void MatrixStack::loadIdentity() {
    top() = re::mat4(1.0f);
  }

  inline void MatrixStack::loadMatrix(const re::mat4& m) {
    top() = m;
  }

//    inline void MatrixStack::frustum(float left ,float right, float btm, float top, float near, float far) {
//      this->top() = re::frustum(left, right, btm, top, near, far);
//    }
}

#endif // MATRIXSTACK_H

