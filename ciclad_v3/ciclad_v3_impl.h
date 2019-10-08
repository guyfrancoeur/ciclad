#ifndef _CICLAD_V3_IMPL_H
#define _CICLAD_V3_IMPL_H

#include <cstdio>   //fopen, printf
#include <cstdlib>  //atol
#include <queue>    //sliding window
#include <cstring>  //strtok
#include <ctime>    //clock_t
#include <iostream> //cout
#include <set>      //set
#include <fstream>  //ofstream, ifstream

//#include "../stx/btree_map.h"
#include "../tlx/container/btree_map.hpp"

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;

//Version 3.x (a la C)
struct node3 {
  node3() : depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  node3(tlx::btree_map<uint, node3 *> _enfant) : enfant(_enfant), gen(std::vector<uint> (2)), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }

  uint depth;
  uint key;  //item
  uint nb_ref;
  uint Cid;
  node3 *parent;
  tlx::btree_map<uint, node3 *> enfant;
  std::vector<uint> gen;
};

struct concept3 {
  concept3() : id(0), supp(0), size(0), deleted(0) {}
  concept3(uint _id, uint _supp, ushort _size, ushort _deleted) : id(_id), supp(_supp), size(_size), deleted(_deleted) {}
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
  T enfant;  //tlx::btree_map<uint, node *>
  NodeA();
  NodeA(T _enfant);
  ~NodeA();
};

template <class T>
NodeA<T>::NodeA() {
  depth = 0; key = 0; nb_ref = 0; Cid = 0; parent = NULL;
}

template <class T>
NodeA<T>::NodeA(T _enfant) {
  enfant = _enfant; depth = 0; key = 0; nb_ref = 0; Cid = 0; parent = NULL;
}

template <class T>
NodeA<T>::~NodeA() {  }

template <class T>
class NodeR {
public:
  uint depth;
  uint key;  //item
  uint nb_ref;
  NodeR *parent;
  T enfant;  //tlx::btree_map<uint, node *>
  uint Cid;  //std::set<uint>
  std::vector<uint> gen; //std::set<uint> or std::vector<uint>
  NodeR();
  NodeR(T _enfant);
  ~NodeR();
};

template <class T>
NodeR<T>::NodeR() {
  depth = 0; key = 0; nb_ref = 0; parent = NULL;
}

template <class T>
NodeR<T>::NodeR(T _enfant) {
  enfant = _enfant; depth = 0; key = 0; nb_ref = 0; parent = NULL;
}
template<class T>
inline NodeR<T>::~NodeR() { }
//------------------------------------------------------------------------------
//Prototype

double add(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid);
double del(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid);

template <class T> void wf(T p, string s);
extern void freeNode3(node3 *n); //v3.0

#endif

