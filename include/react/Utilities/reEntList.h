/**
 * @file
 * Contains the definition of the reEntList utility class
 */
#ifndef RE_ENTLIST_H
#define RE_ENTLIST_H

#include "react/Entities/reEnt.h"
#include "react/Utilities/reLinkedList.h"

/**
 * @ingroup utilities
 * A wrapper around reEnt to hold a query ID
 */

struct reQueryable {
  reQueryable(reEnt* entity) : queryID(0), ent(entity) { }
  reUInt queryID;
  reEnt* ent;
};

/**
 * @ingroup utilities
 * An implementation of a doubly linked, sorted linked list for reEnt, sorted
 * according to the reEnt IDs
 */

class reEntList {
public:
  reEntList(const reWorld* world);
  reEntList(const reEntList& list);
  ~reEntList();
  
  reEntList& operator=(const reEntList& list);
  
  bool add(reQueryable* q);
  bool remove(reQueryable* q);
  void append(const reEntList& list);
  void clear();
  bool empty() const;
  
  reUInt size() const;
  
  reLinkedList<reEnt*> sample(reUInt size) const;
  
  struct Node {
    Node(reQueryable* queryable);
    
    reUInt entID() const;
    
    reQueryable* q;
    Node* next;
    Node* prev;
  };
  
  struct Iterator {
    Iterator(Node* start);
    bool operator!=(const Iterator& iter) const;
    reQueryable& operator*() const;
    const Iterator& operator++();
    Node* node;
  };
  
  class reEntItor {
  public:
    reEntItor(Node* node);
    bool operator!=(const reEntItor& iter) const;
    reEnt& operator*() const;
    const reEntItor& operator++();
  private:
    Node* _node;
  };
  
  Iterator qBegin() const;
  Iterator qEnd() const;
  
  reEntItor begin() const;
  reEntItor end() const;
  
private:
  void appendToNode(Node* node, reQueryable* queryable);
  void prependToNode(Node* node, reQueryable* queryable);
  void detachNode(Node* node);
  
  const reWorld& _world;
  reUInt _size;
  Node* _first;
  Node* _last;
};

inline bool reEntList::empty() const {
  return _size == 0;
}

inline reUInt reEntList::size() const {
  return _size;
}

inline reEntList::reEntItor::reEntItor(reEntList::Node* node) : _node(node) {
  // do nothing
}

inline bool reEntList::reEntItor::operator!=(const reEntList::reEntItor& iter) const {
  return _node != iter._node;
}

inline reEnt& reEntList::reEntItor::operator*() const {
  return *_node->q->ent;
}

inline const reEntList::reEntItor& reEntList::reEntItor::operator++() {
  _node = _node->next;
  return *this;
}

inline reEntList::reEntItor reEntList::begin() const {
  return reEntList::reEntItor(_first);
}

inline reEntList::reEntItor reEntList::end() const {
  return reEntList::reEntItor(nullptr);
}

#endif
