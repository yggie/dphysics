#include "demo/MatrixStack.h"

void MatrixStack::push() {
    if (++_head < MATRIXSTACK_DEFAULT_SIZE) {
        _array[_head].setIdentity();
    } else if (_head < MATRIXSTACK_DEFAULT_SIZE + _heapSize) {
        _heap[_head - MATRIXSTACK_DEFAULT_SIZE].setIdentity();
    } else {
        _increaseHeapSize();
        _heap[_head - MATRIXSTACK_DEFAULT_SIZE].setIdentity();
    }
}

mat4 MatrixStack::getMatrix() {
    if (_cacheHead > _head) {
        _cacheHead = _head;
        _cache.setIdentity();
        for (int i = 0; i < _cacheHead; i++) {
            _cache.preMult(_get(i));
        }
    } else if (_cacheHead < _head) {
        for (int i = _cacheHead + 1; i < _head; i++) {
            _cache.preMult(_get(i));
        }
        _cacheHead = _head;
    }
    return (top() * _cache);
}

void MatrixStack::_increaseHeapSize() {
    mat4* newHeap = new mat4[_heapSize + MATRIXSTACK_DEFAULT_SIZE];
    for (int i = 0; i < _heapSize; i++) {
        newHeap[i] = _heap[i];
    }
    mat4* tmp = _heap;
    _heap = newHeap;
    if (tmp) {
        delete[] tmp;
    }
    _heapSize += MATRIXSTACK_DEFAULT_SIZE;
}

