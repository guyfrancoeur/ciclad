#ifndef _ISCI_H_
#define _ISCI_H_

#include <map>
#include <vector>
#include <set>
#include "stx/btree_map.h" // b+tree library
//v2 #include "robin_map.h" // robin hood hash map library
//v2 #include "hopscotch_map.h" // hopscotch hash map library

typedef unsigned int uint; //32bit
typedef unsigned short ushort; //16bit

using namespace std;

//version 2.x (a la C)
struct node {
  node() : depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  node(stx::btree_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
      /*
  #ifdef _BTREE
    node(stx::btree_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #elif defined(_ROBIN)
    node(tsl::robin_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #elif defined(_HOPSCOTCH)
    node(tsl::hopscotch_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #else
    node(std::map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #endif */
  uint depth;
  uint key;  //item
  uint nb_ref;
  uint Cid;  //Conecpt id
  node *parent;
  stx::btree_map<uint, node *> enfant;
  /*#ifdef _BTREE
    stx::btree_map<uint, node *> enfant;
  #elif defined(_ROBIN)
    tsl::robin_map<uint, node *> enfant;
  #elif defined(_HOPSCOTCH)
    tsl::hopscotch_map<uint, node *> enfant;
  #else
    std::map<uint, node *> enfant;
  #endif */
};

struct concept2 {
  concept2() : id(0), supp(0), size(0) {}
  concept2(uint _id, uint _supp, ushort _size) : id(_id), supp(_supp), size(_size) {}
  concept2(uint _id, uint _supp, ushort _size, std::vector<uint> _itemset) : id(_id), supp(_supp), size(_size), itemset(_itemset) {}
  uint id;
  uint supp;
  ushort size;
  std::vector<uint> itemset;
};
//------------------------------------------------------------------------------
//Version 3.x (a la C)
struct node3 {
  node3() : depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  node3(stx::btree_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }

  uint depth;
  uint key;  //item
  uint nb_ref;
  uint Cid;  //Conecpt id
  node *parent;
  stx::btree_map<uint, node *> enfant;
  std::vector<uint> gen;
};

struct concept3 {
  concept3() : id(0), supp(0), size(0), deleted(0){}
  concept3(uint _id, uint _supp, ushort _size, ushort _deleted) : id(_id), supp(_supp), size(_size), deleted(_deleted){}
  concept3(uint _id, uint _supp, ushort _size, ushort _deleted, std::vector<uint> _itemset) : id(_id), supp(_supp), size(_size), deleted(_deleted), itemset(_itemset) {}
  uint id;
  uint supp;
  ushort size;
  ushort deleted; //flag 0 | 1
  std::vector<uint> itemset;
};

//------------------------------------------------------------------------------
//version 3.x (a la C++)
template <class T>
class NodeA {
 public:
  uint depth;
  uint key;    //item
  uint nb_ref;
  uint Cid;   //Concept id
  NodeA *parent;
  T enfant;  //stx::btree_map<uint, node *>
  NodeA();
  NodeA(T _enfant);
  ~NodeA();
};

template <class T>
NodeA<T>::NodeA(){
  depth = 0; key = 0; nb_ref = 0; Cid = 0; parent = NULL;
}

template <class T>
NodeA<T>::NodeA(T _enfant){
  enfant = _enfant; depth = 0; key = 0; nb_ref = 0; Cid = 0; parent = NULL;
}

template <class T>
NodeA<T>::~NodeA() { /* free() or delete[] */ }
//------------------------------------------------------------------------------
template <class T>
class NodeR {
 public:
  uint depth;
  uint key;  //item
  uint nb_ref;
  NodeR *parent;
  T enfant; //stx::btree_map<uint, node *>
  std::set<uint> Cid;
  std::set<uint> gen; //std::set<uint> or std::vector<uint>
  NodeR();
  NodeR(T _enfant);
  ~NodeR();
};

template <class T>
NodeR<T>::NodeR(){
  depth = 0; key = 0; nb_ref = 0; parent = NULL;
}

template <class T>
NodeR<T>::NodeR(T _enfant){
  enfant = _enfant; depth = 0; key = 0; nb_ref = 0; parent = NULL;
}

template <class T>
NodeR<T>::~NodeR() { /* free() or delete[] */ }

//------------------------------------------------------------------------------
template <class T> void wf(T p, string s);
extern void freeNode(node *n);   // v2.1h
extern void freeNode3(node3 *n); //v3.0

#endif // _ISCI_H_
