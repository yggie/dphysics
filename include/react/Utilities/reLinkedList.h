/**
 * @file
 * Contains the definition of the reLinkedNode, reIterator and reLinkedList class
 */
#ifndef RE_LINKEDLIST_H
#define RE_LINKEDLIST_H

#include "react/common.h"
#include "react/Memory/reAllocator.h"

/**
 * @ingroup utilities
 * Represents a single node in the linked list
 */

template<class T>
class reLinkedNode {
public:
  reLinkedNode();
  
  T& value();
  const T& value() const;
  reLinkedNode*& next();
  const reLinkedNode*& next() const;
  
private:
  T _value;
  reLinkedNode<T>* _next;
};

/**
 * @ingroup utilities
 * An iterator to iterate over elements in the list
 */

template <class T>
class reIterator {
public:
  reIterator(reLinkedNode<T>* start);
  bool operator!=(const reIterator& iter) const;
  T& operator*() const;
  const reIterator& operator++();
  reLinkedNode<T>* node;
};

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
  
  bool add(T value);
  bool remove(T value);
  void append(const reLinkedList<T>& list);
  void clear();
  bool empty() const;
  
  reIterator<T> begin() const;
  reIterator<T> end() const;
  
private:
  reAllocator& _allocator;
  reLinkedNode<T>* _first;
  reLinkedNode<T>* _last;
};

template <class T>
reLinkedNode<T>::reLinkedNode() : _value(), _next(nullptr) {
  // do nothing
}

template <class T>
T& reLinkedNode<T>::value() {
  return _value;
}

template <class T>
inline const T& reLinkedNode<T>::value() const {
  return _value;
}

template <class T>
inline reLinkedNode<T>*& reLinkedNode<T>::next() {
  return _next;
}

template <class T>
inline const reLinkedNode<T>*& reLinkedNode<T>::next() const {
  return _next;
}

template <class T>
reLinkedList<T>::reLinkedList(reAllocator& allocator) : _allocator(allocator), _first(nullptr), _last(nullptr) {
  // do nothing
}

template <class T>
reLinkedList<T>::reLinkedList(const reLinkedList<T>& list) : _allocator(list._allocator), _first(nullptr), _last(nullptr) {
  append(list);
}

template <class T>
reLinkedList<T>::~reLinkedList() {
  clear();
}

template <class T>
inline bool reLinkedList<T>::add(T t) {
  reLinkedNode<T>* node = _allocator.alloc_new<reLinkedNode<T>>();
  node->value() = t;
  // TODO sort values
  if (_first == nullptr) {
    _first = node;
    _last = node;
  } else {
    _last->next() = node;
    _last = node;
  }
  return true;
}

template <class T>
inline bool reLinkedList<T>::remove(T t) {
  reLinkedNode<T>* node = _first;
  reLinkedNode<T>* prevNode = nullptr;
  while (node != nullptr) {
    if (node->value() == t) {
      if (prevNode == nullptr) {
        _first = node->next();
      } else {
        prevNode->next() = node->next();
        if (prevNode->next() == nullptr) {
          _last = prevNode;
        }
      }
      _allocator.alloc_delete<reLinkedNode<T>>(node);
      
      return true;
    }
    prevNode = node;
    node = node->next();
  }
  
  return false;
}

template <class T>
inline void reLinkedList<T>::append(const reLinkedList<T>& list) {
  for (T t : list) {
    add(t);
  }
}

template <class T>
inline bool reLinkedList<T>::empty() const {
  return _first == nullptr;
}

template <class T>
inline void reLinkedList<T>::clear() {
  reLinkedNode<T>* node = _first;
  while (node != nullptr) {
    reLinkedNode<T>*tmp = node->next();
    _allocator.alloc_delete(node);
    node = tmp;
  }
  _first = _last = nullptr;
}

template <class T>
reIterator<T>::reIterator(reLinkedNode<T>* start) : node(start) {
  // do nothing
}

template <class T>
bool reIterator<T>::operator!=(const reIterator<T>& iter) const {
  return node != iter.node;
}

template <class T>
T& reIterator<T>::operator*() const {
  return node->value();
}

template <class T>
const reIterator<T>& reIterator<T>::operator++() {
  node = node->next();
  return *this;
}

template <class T>
reIterator<T> reLinkedList<T>::begin() const {
  return reIterator<T>(_first);
}

template <class T>
reIterator<T> reLinkedList<T>::end() const {
  return reIterator<T>(nullptr);
}


#endif
