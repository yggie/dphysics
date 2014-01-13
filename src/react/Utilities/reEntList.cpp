#include "react/Utilities/reEntList.h"

#include <cstdlib>

namespace {
  reUInt* genRandomIndices(reUInt size, reUInt maxValue);
}

reEntList::reEntList() : _size(0), _first(nullptr), _last(nullptr) {
  // do nothing
}

reEntList::reEntList(const reEntList& list) : reEntList() {
  append(list);
}

reEntList::~reEntList() {
  clear();
}

reEntList& reEntList::operator=(const reEntList& list) {
  clear();
  append(list);
  return *this;
}

bool reEntList::add(reQueryable* q) {
  if (_last != nullptr && _last->entID() < q->ent->id()) {
    _last->append(q);
    _last = _last->next;
    _size++;
    return true;
  }
  
  if (_first == nullptr) {
    _first = re::alloc_new<Node>(q);
    _last = _first;
    _size++;
    return true;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  while (node != nullptr) {
    if (node->entID() > q->ent->id()) {
      node->prepend(q);
      if (node == _first) {
        _first = node->prev;
      }
      _size++;
      return true;
    } else if (node->entID() == q->ent->id()) {
      return false;
    }
    node = node->next;
  }
  
  RE_IMPOSSIBLE
  
  _last->append(q);
  _last = _last->next;
  _size++;
  return true;
}

bool reEntList::remove(reQueryable* q) {
  if (_first == nullptr ||
      _first->entID() > q->ent->id() ||
      _last->entID() < q->ent->id()) {
    return false;
  }
  
  if (_first == _last && _first->entID() == q->ent->id()) {
    re::alloc_delete(_first);
    _first = nullptr;
    _last = nullptr;
    _size--;
    return true;
  } else if (_first->entID() == q->ent->id()) {
    Node* node = _first;
    _first = _first->next;
    node->unlink();
    re::alloc_delete(node);
    _size--;
    return true;
  } else if (_last->entID() == q->ent->id()) {
    Node* node = _last;
    _last = _last->prev;
    node->unlink();
    re::alloc_delete(node);
    _size--;
    return true;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  while (node != nullptr) {
    if (node->entID() == q->ent->id()) {
      node->unlink();
      re::alloc_delete(node);
      _size--;
      return true;
    }
    node = node->next;
  }
  
  return false;
}

void reEntList::append(const reEntList& list) {
  if (empty() || list._first->entID() > _last->entID()) {
    for (reQueryable& q : list) {
      add(&q);
    }
    return;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  Node* thatNode = list._first;
  while (node != nullptr && thatNode != nullptr) {
    if (node->entID() > thatNode->entID()) {
      if (node->prev->entID() != thatNode->entID()) {
        node->prepend(thatNode->q);
        _size++;
      } else {
        thatNode = thatNode->next;
      }
    } else {
      node = node->next;
    }
  }
  
  if (node == nullptr) {
    while (thatNode != nullptr) {
      _last->append(thatNode->q);
      _last = _last->next;
      _size++;
      thatNode = thatNode->next;
    }
  }
}

void reEntList::clear() {
  Node* node = _first;
  while (node != nullptr) {
    Node* tmp = node;
    node = tmp->next;
    delete tmp;
  }
  
  _first = nullptr;
  _last = nullptr;
  _size = 0;
}

reLinkedList<reEnt*> reEntList::sample(reUInt size) const {
  reLinkedList<reEnt*> list;
  
  reUInt* indices = genRandomIndices(size, _size);
  
  Node* node = _first;
  reUInt idx = 0;
  reUInt i = 0;
  while (node != nullptr) {
    if (i++ == indices[idx]) {
      list.add(node->q->ent);
      if (++idx == size) {
        break;
      }
    }
    node = node->next;
  }
  
  RE_ASSERT(idx == size, "Loop incomplete")
  
  delete indices;
  return list;
}

reEntList::Node::Node(reQueryable* queryable)
  : q(queryable), next(nullptr), prev(nullptr) {
  // do nothing
}

reUInt reEntList::Node::entID() const {
  return q->ent->id();
}

void reEntList::Node::append(reQueryable* q) {
  Node* node = re::alloc_new<Node>(q);
  node->prev = this;
  if (next != nullptr) {
    next->prev = node;
    node->next = next;
  }
  next = node;
}

void reEntList::Node::prepend(reQueryable* q) {
  Node* node = re::alloc_new<Node>(q);
  node->next = this;
  if (prev != nullptr) {
    prev->next = node;
    node->prev = prev;
  }
  prev = node;
}

void reEntList::Node::unlink() {
  if (prev != nullptr) {
    prev->next = next;
  }
  if (next != nullptr) {
    next->prev = prev;
  }
}

reEntList::Iterator::Iterator(Node* start) : node(start) {
  // do nothing
}

bool reEntList::Iterator::operator!=(const reEntList::Iterator& iter) const {
  return node != iter.node;
}

reQueryable& reEntList::Iterator::operator*() const {
  return *node->q;
}

const reEntList::Iterator& reEntList::Iterator::operator++() {
  node = node->next;
  return *this;
}

reEntList::Iterator reEntList::begin() const {
  return reEntList::Iterator(_first);
}

reEntList::Iterator reEntList::end() const {
  return reEntList::Iterator(nullptr);
}

namespace {
  reUInt* genRandomIndices(reUInt size, reUInt maxValue) {
    reUInt* array = new reUInt[size];
    
    if (size == 1) {
      array[0] = rand() % maxValue;
      return array;
    }
    
    reUInt i1 = 0;
    reUInt i2 = size - 1;
    reUInt v1 = rand() % maxValue;
    reUInt v2 = rand() % maxValue;
    while (v2 == v1) {
      v2 = rand() % maxValue;
    }
    
    if (v1 < v2) {
      array[i1] = v1;
      array[i2] = v2;
    } else {
      array[i1] = v2;
      array[i2] = v1;
    }
    
    if (size == 2) {
      return array;
    }
    
    // loop for each remaining numbers
    for (reUInt i = 2; i < size; i++) {
      reUInt val = rand() % maxValue;
      
      // check if it is repeated at the heads
      if (val == array[i1] || val == array[i2]) {
        i--;
        continue;
      }
      
      // if possible candidate for first head
      if (val < array[i1]) {
      
        if (i1 == 0) {
          array[1] = array[0];
          array[0] = val;
          i1 = 1;
          continue;
        }
        
        bool success = false;
        
        for (reUInt j = i1; j > 0; j--) {
          if (val < array[j] && val > array[j - 1]) {
            for (reUInt k = ++i1; k > j; k--) {
              array[k] = array[k - 1];
            }
            array[j] = val;
            success = true;
            break;
          } else if (val == array[j - 1]) {
            i--;
            success = true;
            break;
          }
        }
        
        if (!success) {
          for (reUInt k = ++i1; k > 0; k--) {
            array[k] = array[k - 1];
          }
          array[0] = val;
        }
        
      } else if (val > array[i2] ) {
      
        if (i2 == 0) {
          array[size - 2] = array[size - 1];
          array[size - 1] = val;
          i2 = size - 2;
          continue;
        }
        
        bool success = false;
        
        for (reUInt j = i2; j < size - 1; j++) {
          if (val > array[j] && val < array[j + 1]) {
            for (reUInt k = --i2; k < j; k++) {
              array[k] = array[k + 1];
            }
            array[j] = val;
            success = true;
            break;
          } else if (val == array[j + 1]) {
            i--;
            success = true;
            break;
          }
        }
        
        if (!success) {
          for (reUInt k = --i2; k < size - 1; k++) {
            array[k] = array[k + 1];
          }
          array[size - 1] = val;
        }
        
      } else {
        array[++i1] = val;
      }
    }
    
    return array;
  }
}

