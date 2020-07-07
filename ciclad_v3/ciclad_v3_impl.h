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
#include <thread>   //thread (clean idx, CI)
#include <omp.h>    //openmp

//#include "../stx/btree_map.h"
#include "../tlx/container/btree_map.hpp"

using namespace std;

typedef unsigned int uint;
typedef unsigned short ushort;

class Stats {
public:
  uint rows_processed;
  uint rows_inserted;
  uint rows_removed;
  double avg_ms_insert;
  double avg_ms_remove;
  Stats() { rows_processed = 0; rows_inserted = 0; rows_removed = 0; avg_ms_insert = 0; avg_ms_remove = 0; };
  ~Stats() { };
  void insert(const double *_ms) {
    ++rows_inserted;
    avg_ms_insert = (avg_ms_insert + *_ms) / 2;
  };
  void remove(const double *_ms) {
    ++rows_removed;
    avg_ms_remove = (avg_ms_remove + *_ms) / 2;
  };
};

//Version 3.x (a la C)
struct node3 {
  node3() : depth(0), key(0), nb_ref(0), Cid(0), enfant(tlx::btree_map<uint, node3 *>()), parent(NULL) { }
  node3(tlx::btree_map<uint, node3 *> _enfant) : enfant(_enfant), gen(std::vector<uint> (2)), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }

  uint depth;
  uint key;  //item
  uint nb_ref;
  uint Cid;
  node3 *parent;
  tlx::btree_map<uint, node3 *> enfant;
  std::vector<uint> gen;
  std::vector<uint> cid; //pour le delete uniquement
};

struct concept3 {
  concept3() : id(0), supp(0), deleted(0) {}
  concept3(uint _id, uint _supp, ushort _deleted) : id(_id), supp(_supp), deleted(_deleted) {}
  concept3(uint _id, uint _supp, ushort _deleted, std::vector<uint> _itemset) : id(_id), supp(_supp), deleted(_deleted), itemset(_itemset) {}
  uint id; // utile si la fCI2 n'est pas un vector todo : optimisation
  uint supp;
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
double cleanup(std::vector<vector<uint>>& _idx, std::vector<concept3>& _fCI2);
double add(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid);
double del(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid);

template <class T> void wf(T p, string s);
extern void freeNode3(node3 *n); //v3.0

#endif
