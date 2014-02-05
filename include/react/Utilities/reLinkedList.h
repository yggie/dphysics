/**
 * @file
 * Contains the definition of the reLinkedList class
 */
#ifndef RE_LINKEDLIST_H
#define RE_LINKEDLIST_H

#include "react/common.h"
#include "react/Memory/reAllocator.h"

/**
 * @ingroup utilities
 * An implementation of a simple linked list
 */

template <class T>
class reLinkedList {
public:
  reLinkedList(reAllocator& allocator);
  reLinkedList(const reLinkedList& list);
  ~reLinkedList();
  
  reLinkedList& operator=(const reLinkedList& list);
  
  struct Node {
    Node(const T& v) : value(v), next(nullptr) { }
    T value;
    Node* next;
  };

  struct iterator {
    iterator(Node* start);
    bool operator!=(const iterator& iter) const;
    T& operator*() const;
    const iterator& operator++();
    Node* node;
  };
  
  void add(T value);
  bool remove(T value);
  bool contains(const T& value) const;
  void append(const reLinkedList<T>& list);
  void clear();
  bool empty() const;
  reUInt size() const;
  
  iterator begin() const;
  iterator end() const;
  
private:
  reAllocator& _allocator;
  Node* _first;
  Node* _last;
  reUInt _size;
};

template <class T>
reLinkedList<T>::reLinkedList(reAllocator& allocator) : _allocator(allocator), _first(nullptr), _last(nullptr), _size(0) {
  // do nothing
}

template <class T>
reLinkedList<T>::reLinkedList(const reLinkedList<T>& list) : reLinkedList(list._allocator) {
  append(list);
}

template <class T>
reLinkedList<T>::~reLinkedList() {
  clear();
}

template <class T>
void reLinkedList<T>::add(T t) {
  Node* node = _allocator.alloc_new<Node>(t);
  if (_first == nullptr) {
    _first = node;
    _last = node;
  } else {
    _last->next = node;
    _last = node;
  }
  _size++;
}

template <class T>
reLinkedList<T>& reLinkedList<T>::operator=(const reLinkedList<T>& list) {
  clear();
  append(list);
}

template <class T>
bool reLinkedList<T>::remove(T t) {
  Node* node = _first;
  Node* prevNode = nullptr;
  while (node != nullptr) {
    if (node->value == t) {
      if (prevNode == nullptr) {
        _first = node->next;
      } else {
        prevNode->next = node->next;
        if (prevNode->next == nullptr) {
          _last = prevNode;
        }
      }
      _allocator.alloc_delete<Node>(node);
      
      _size--;
      return true;
    }
    prevNode = node;
    node = node->next;
  }
  
  return false;
}

template <class T>
bool reLinkedList<T>::contains(const T& t) const {
  const Node* node = _first;
  while (node != nullptr) {
    if (node->value == t) {
      return true;
    }
    node = node->next;
  }
  
  return false;
}

template <class T>
void reLinkedList<T>::append(const reLinkedList<T>& list) {
  for (T t : list) {
    add(t);
  }
}

template <class T>
inline bool reLinkedList<T>::empty() const {
  return _first == nullptr;
}

template <class T>
inline reUInt reLinkedList<T>::size() const {
  return _size;
}

template <class T>
void reLinkedList<T>::clear() {
  Node* node = _first;
  while (node != nullptr) {
    Node* tmp = node->next;
    _allocator.alloc_delete(node);
    node = tmp;
  }
  _first = _last = nullptr;
  _size = 0;
}

template <class T>
reLinkedList<T>::iterator::iterator(Node* start) : node(start) {
  // do nothing
}

template <class T>
inline bool reLinkedList<T>::iterator::operator!=(const iterator& iter) const {
  return node != iter.node;
}

template <class T>
inline T& reLinkedList<T>::iterator::operator*() const {
  return node->value;
}

template <class T>
inline const typename reLinkedList<T>::iterator& reLinkedList<T>::iterator::operator++() {
  node = node->next;
  return *this;
}

template <class T>
inline typename reLinkedList<T>::iterator reLinkedList<T>::begin() const {
  return iterator(_first);
}

template <class T>
inline typename reLinkedList<T>::iterator reLinkedList<T>::end() const {
  return iterator(nullptr);
}


#endif
