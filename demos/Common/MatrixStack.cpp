#include "MatrixStack.h"

#include <cassert>

using namespace re::demo;

MatrixStack::MatrixStack(int initialCapacity) : _head(0),
_cacheHead(-1), _capacity(initialCapacity), _heap(nullptr),
_cached(1.0f) {
  _heap = new re::mat4[initialCapacity];
  _heap[0] = re::mat4(1.0f);
}

MatrixStack::~MatrixStack() {
  if (_heap) {
    delete[] _heap;
  }
}

const re::mat4 MatrixStack::mat() {
  assert((_cachedHead < _head) && "Cache must always be behind the head");
  if (_cacheHead < _head) {
    for (int i = _cacheHead + 1; i < _head; i++) {
      _cached = get(i) * _cached;
    }
    _cacheHead = _head - 1;
  }
  return (top() * _cached);
}

void MatrixStack::increaseHeap() {
  re::mat4* newHeap = new re::mat4[_capacity + MATRIXSTACK_DEFAULT_CAPACITY];
  for (int i = 0; i < _capacity; i++) {
    newHeap[i] = _heap[i];
  }
  re::mat4* tmp = _heap;
  _heap = newHeap;
  if (tmp) {
    delete[] tmp;
  }
  _capacity += MATRIXSTACK_DEFAULT_CAPACITY;
}
