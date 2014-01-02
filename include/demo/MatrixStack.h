#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "demo/mat4.h"

#define MATRIXSTACK_DEFAULT_SIZE    5

class MatrixStack {
public:
    MatrixStack();                                          // inline
    ~MatrixStack();                                         // inline

    int size() const;                                       // inline
    void clear();                                           // inline

    void push();                                            // inline
    void pop();                                             // inline
    mat4& top();                                            // inline

    void scale(float s);                                    // inline
    void scale(float x, float y, float z);                  // inline
    void translate(float x, float y, float z);              // inline
    void rotate(float angle, float x, float y, float z);    // inline
    void rotateZ(float angle);                              // inline
    void loadIdentity();                                    // inline

    mat4 getMatrix();                                       // inline
    void setMatrix(const mat4& mat);                        // inline

private:
    mat4& _get(int i);                                      // inline
    void _increaseHeapSize();

    int _head;
    int _cacheHead;
    int _heapSize;
    mat4* _heap;
    mat4 _cache;
    mat4 _array[MATRIXSTACK_DEFAULT_SIZE];
};

inline MatrixStack::MatrixStack() : _head(0), _cacheHead(-1), _heapSize(0), _heap(NULL) {
}

inline MatrixStack::~MatrixStack() {
    if (_heap) {
        delete[] _heap;
    }
}

inline int MatrixStack::size() const {
    return (_head + 1);
}

inline void MatrixStack::clear() {
    _head = 0;
    _array[0] = mat4::IDENTITY;
}

inline void MatrixStack::pop() {
    if (_head == 0) {
        _array[0].setIdentity();
    } else {
        _head--;
    }
}

inline mat4& MatrixStack::top() {
    return _get(_head);
}

inline void MatrixStack::scale(float s) {
    top().scale(s);
}

inline void MatrixStack::scale(float x, float y, float z) {
    top().scale(x, y, z);
}

inline void MatrixStack::translate(float x, float y, float z) {
    top().translate(x, y, z);
}

inline void MatrixStack::rotate(float angle, float x, float y, float z) {
    top().rotate(angle, x, y, z);
}

inline void MatrixStack::rotateZ(float angle) {
    top().rotateZ(angle);
}

inline void MatrixStack::loadIdentity() {
    top().setIdentity();
}

inline mat4& MatrixStack::_get(int i) {
    if (i < MATRIXSTACK_DEFAULT_SIZE) {
        return _array[i];
    } else {
        return _heap[i - MATRIXSTACK_DEFAULT_SIZE];
    }
}

inline void MatrixStack::setMatrix(const mat4& mat) {
    top() = mat;
}

#endif // MATRIXSTACK_H

