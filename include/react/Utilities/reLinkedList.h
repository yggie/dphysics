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
struct reLinkedNode {
  reLinkedNode() : value(), next(nullptr) { }
  
  T value;
  reLinkedNode<T>* next;
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
  
  void add(T value);
  bool remove(T value);
  bool contains(const T& value) const;
  void append(const reLinkedList<T>& list);
  void clear();
  bool empty() const;
  reUInt size() const;
  
  reIterator<T> begin() const;
  reIterator<T> end() const;
  
private:
  reAllocator& _allocator;
  reLinkedNode<T>* _first;
  reLinkedNode<T>* _last;
  reUInt _size;
};

template <class T>
reLinkedList<T>::reLinkedList(reAllocator& allocator) : _allocator(allocator), _first(nullptr), _last(nullptr), _size(0) {
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
inline void reLinkedList<T>::add(T t) {
  reLinkedNode<T>* node = _allocator.alloc_new<reLinkedNode<T>>();
  node->value = t;
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
inline bool reLinkedList<T>::remove(T t) {
  reLinkedNode<T>* node = _first;
  reLinkedNode<T>* prevNode = nullptr;
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
      _allocator.alloc_delete<reLinkedNode<T>>(node);
      
      _size--;
      return true;
    }
    prevNode = node;
    node = node->next;
  }
  
  return false;
}

template <class T>
inline bool reLinkedList<T>::contains(const T& t) const {
  const reLinkedNode<T>* node = _first;
  while (node != nullptr) {
    if (node->value == t) {
      return true;
    }
    node = node->next;
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
inline reUInt reLinkedList<T>::size() const {
  return _size;
}

template <class T>
inline void reLinkedList<T>::clear() {
  reLinkedNode<T>* node = _first;
  while (node != nullptr) {
    reLinkedNode<T>*tmp = node->next;
    _allocator.alloc_delete(node);
    node = tmp;
  }
  _first = _last = nullptr;
  _size = 0;
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
  return node->value;
}

template <class T>
const reIterator<T>& reIterator<T>::operator++() {
  node = node->next;
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
