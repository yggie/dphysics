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
  reEntList();
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
    void append(reQueryable* q);
    void prepend(reQueryable* q);
    void unlink();
    
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
  
  Iterator begin() const;
  Iterator end() const;
  
private:
  
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

#endif
