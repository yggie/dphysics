#include "react/Utilities/reEntList.h"

#include <cstdlib>

namespace {
  reUInt* genRandomIndices(reUInt size, reUInt maxValue);
}

reEntList::reEntList(reAllocator& allocator) : _allocator(allocator), _size(0), _first(nullptr), _last(nullptr) {
  // do nothing
}

reEntList::reEntList(const reEntList& list) : reEntList(list._allocator) {
  clear();
  append(list);
}

reEntList::~reEntList() {
  printf("CLEAR=%d\n", size());
  clear();
}

reEntList& reEntList::operator=(const reEntList& list) {
  clear();
  append(list);
  return *this;
}

bool reEntList::add(reQueryable& q) {
  RE_ASSERT(&q.ent != nullptr)
  if (_last != nullptr && _last->entID() < q.ent.id()) {
    appendToNode(_last, q);
    _last = _last->next;
    _size++;
    return true;
  }
  
  if (_first == nullptr) {
    _first = _allocator.alloc_new<Node>(q);
    _last = _first;
    _size++;
    return true;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  while (node != nullptr) {
    if (node->entID() > q.ent.id()) {
      prependToNode(node, q);
      if (node == _first) {
        _first = node->prev;
      }
      _size++;
      return true;
    } else if (node->entID() == q.ent.id()) {
      return false;
    }
    node = node->next;
  }
  
  RE_IMPOSSIBLE
  
  appendToNode(_last, q);
  _last = _last->next;
  _size++;
  return true;
}

bool reEntList::remove(reQueryable& q) {
  if (_first == nullptr ||
      _first->entID() > q.ent.id() ||
      _last->entID() < q.ent.id()) {
    return false;
  }
  
  if (_first == _last && _first->entID() == q.ent.id()) {
    _allocator.alloc_delete(_first);
    _first = nullptr;
    _last = nullptr;
    _size--;
    return true;
  } else if (_first->entID() == q.ent.id()) {
    Node* node = _first;
    _first = _first->next;
    detachNode(node);
    _allocator.alloc_delete(node);
    _size--;
    return true;
  } else if (_last->entID() == q.ent.id()) {
    Node* node = _last;
    _last = _last->prev;
    detachNode(node);
    _allocator.alloc_delete(node);
    _size--;
    return true;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  while (node != nullptr) {
    if (node->entID() == q.ent.id()) {
      detachNode(node);
      _allocator.alloc_delete(node);
      _size--;
      return true;
    }
    node = node->next;
  }
  
  return false;
}

void reEntList::remove(const reEntList& list) {
  auto end = list.qEnd();
  for (auto iter = list.qBegin(); iter != end; ++iter) {
    remove(*iter);
  }
}

bool reEntList::contains(const reEnt& ent) const {
  for (const reEnt& e : *this) {
    if (e.id() == ent.id()) {
      return true;
    }
  }
  return false;
}

void reEntList::append(const reEntList& list) {
  if (list.empty()) {
    return;
  }
  
  // can append all elements to the front
  if (empty() || 
      list._first->entID() > _last->entID()) {
    printf("APPENDING=%d\n", list.size());
    auto end = list.qEnd();
    int i = 0;
    for (auto it = list.qBegin(); it != end; ++it) {
      add(*it);
      printf("@@@AADSA@@@ +++++ %d\n", ++i);
    }
    return;
  }
  
  // TODO perform backward or forward search depending on difference in ID
  Node* node = _first;
  Node* thatNode = list._first;
  while (node != nullptr && thatNode != nullptr) {
    if (node->entID() < thatNode->entID()) {
      node = node->next;
    } else if (node->entID() > thatNode->entID()) {
      RE_ASSERT(node->prev == nullptr ||
                node->prev->entID() < thatNode->entID())
      prependToNode(node, thatNode->q);
      _size++;
      thatNode = thatNode->next;
      if (node == _first) {
        _first = node->prev;
      }
    } else {
      node = node->next;
      thatNode = thatNode->next;
    }
  }
  
  if (node == nullptr) {
    RE_ASSERT(thatNode == nullptr || _last->entID() < thatNode->entID())
    while (thatNode != nullptr) {
      appendToNode(_last, thatNode->q);
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
    _allocator.alloc_delete(tmp);
  }
  
  _first = nullptr;
  _last = nullptr;
  _size = 0;
}

reLinkedList<reEnt*> reEntList::sample(reUInt size) const {
  reLinkedList<reEnt*> list(_allocator);
  
  reUInt* indices = genRandomIndices(size, _size);
  
  Node* node = _first;
  reUInt idx = 0;
  reUInt i = 0;
  while (node != nullptr) {
    if (i++ == indices[idx]) {
      list.add(&node->q.ent);
      if (++idx == size) {
        break;
      }
    }
    node = node->next;
  }
  
  RE_ASSERT_MSG(idx == size, "Loop incomplete")
  
  delete indices;
  return list;
}

reEntList::Node::Node(reQueryable& queryable)
  : q(queryable), next(nullptr), prev(nullptr) {
  // do nothing
}

reUInt reEntList::Node::entID() const {
  return q.ent.id();
}

void reEntList::appendToNode(Node* node, reQueryable& q) {
  Node* newNode = _allocator.alloc_new<Node>(q);
  newNode->prev = node;
  if (node->next != nullptr) {
    node->next->prev = newNode;
    newNode->next = node->next;
  }
  node->next = newNode;
}

void reEntList::prependToNode(Node* node, reQueryable& q) {
  Node* newNode = _allocator.alloc_new<Node>(q);
  newNode->next = node;
  if (node->prev != nullptr) {
    node->prev->next = newNode;
    newNode->prev = node->prev;
  }
  node->prev = newNode;
}

void reEntList::detachNode(Node* node) {
  if (node->prev != nullptr) {
    node->prev->next = node->next;
  }
  if (node->next != nullptr) {
    node->next->prev = node->prev;
  }
}

namespace {
  reUInt* genRandomIndices(reUInt size, reUInt maxValue) {
    reUInt* array = new reUInt[size];
    
    if (size == 1) {
      array[0] = re::randi() % maxValue;
      return array;
    }
    
    reUInt i1 = 0;
    reUInt i2 = size - 1;
    reUInt v1 = re::randi() % maxValue;
    reUInt v2 = re::randi() % maxValue;
    while (v2 == v1) {
      v2 = re::randi() % maxValue;
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
      reUInt val = re::randi() % maxValue;
      
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

