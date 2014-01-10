/**
 * @file
 * Contains the definition of the reLinkedNode and reLinkedList class
 */
#ifndef RE_LINKEDLIST_H
#define RE_LINKEDLIST_H

#include "react/common.h"

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
  reLinkedNode* _next;
};

template <class T>
class Iterator {
public:
  Iterator(reLinkedNode<T>* start);
  bool operator!=(const Iterator& iter) const;
  T& operator*() const;
  const Iterator& operator++();
  reLinkedNode<T>* node;
};

/**
 * @ingroup utilities
 * An implementation of a sorted linked list
 */

template <class T>
class reLinkedList {
public:
  reLinkedList();
  reLinkedList(const reLinkedList& list);
  ~reLinkedList();
  
  bool add(T value);
  bool remove(T value);
  void append(const reLinkedList<T>& list);
  void clear();
  bool empty() const;
  
  Iterator<T> begin() const;
  Iterator<T> end() const;
  
private:
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
reLinkedList<T>::reLinkedList() : _first(nullptr), _last(nullptr) {
  // do nothing
}

template <class T>
reLinkedList<T>::reLinkedList(const reLinkedList<T>& list) : _first(nullptr), _last(nullptr) {
  append(list);
}

template <class T>
reLinkedList<T>::~reLinkedList() {
  clear();
}

template <class T>
inline bool reLinkedList<T>::add(T t) {
  reLinkedNode<T>* node = re::alloc_new<reLinkedNode<T>>();
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
      re::alloc_delete<reLinkedNode<T>>(node);
      
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
    re::alloc_delete(node);
    node = tmp;
  }
  _first = _last = nullptr;
}

template <class T>
Iterator<T>::Iterator(reLinkedNode<T>* start) : node(start) {
  // do nothing
}

template <class T>
bool Iterator<T>::operator!=(const Iterator<T>& iter) const {
  return node != iter.node;
}

template <class T>
T& Iterator<T>::operator*() const {
  return node->value();
}

template <class T>
const Iterator<T>& Iterator<T>::operator++() {
  node = node->next();
  return *this;
}

template <class T>
Iterator<T> reLinkedList<T>::begin() const {
  return Iterator<T>(_first);
}

template <class T>
Iterator<T> reLinkedList<T>::end() const {
  return Iterator<T>(nullptr);
}


#endif
