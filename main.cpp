// version 2.1h 2018-09-27 based on Galicia-P (codename isci) by Guy Francoeur
// G++ v4.8.x et + (4.9.x ideal): g++ -Wall -DNDEBUG -static -O3 -std=c++11 isci.cpp -I ./ -o isci
// Test : valgrind --tool=memcheck --leak-check=yes ./isci ~/isci/testdb_gen2.txt
// Profiler : valgrind --tools=callgrind ./isci ~/isci/testdb_gen2.txt
// fastest testdb_gen2.txt 97sec
// TODO : cmdline() [-i <in stream/file> | default stdin] [-o <output stream/file> | default stdout] [-w <window size> | default 1000] [-t <window time> | default 0] [-r <ram limit> | default 50%]
// TODO : sliding window
// TODO : remove from window
// TODO : fix the index initial capacity to be variable
// TODO : validate uint to be 64bit
//PREPROCESSORS (also for isci.h)
#define _BTREE  // _BTREE | _ROBIN | _HOPSCOTCH | _STLMAP_a // structure pour la trie
#define _VECTOR // _VECTOR | _STLMAP_b         // structure pour les concepts

#include <cstdio>   //fopen
#include <cstdlib>  //printf
#include <queue>    //sliding window
#include <cstring>  //strtok
#include <ctime>    //clock_t
#include <iostream> //cout
#include "stx/btree_map.h" // b+tree library
#include "robin_map.h" // robin hood hash map library
#include "hopscotch_map.h" // hopscotch hash map library
#include "isci.h"  //cmdline

#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
#endif

using namespace std;

struct node {
  node() : depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #ifdef _BTREE
    node(stx::btree_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #elif defined(_ROBIN)
    node(tsl::robin_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #elif defined(_HOPSCOTCH)
    node(tsl::hopscotch_map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #else
    node(std::map<uint, node *> _enfant) : enfant(_enfant), depth(0), key(0), nb_ref(0), Cid(0), parent(NULL) { }
  #endif
  uint depth;
  uint key;  //item
  uint nb_ref;
  uint Cid;
  node *parent;
  #ifdef _BTREE
    stx::btree_map<uint, node *> enfant;
  #elif defined(_ROBIN)
    tsl::robin_map<uint, node *> enfant;
  #elif defined(_HOPSCOTCH)
    tsl::hopscotch_map<uint, node *> enfant;
  #else
    std::map<uint, node *> enfant;
  #endif
};

void freeNode(node *n) {
  auto _enfant = n->enfant;
  auto it1 = _enfant.begin();
  while (it1 != _enfant.end()) {
    if (_enfant.size() > 0) {
      freeNode(it1->second);
    }
    ++it1;
  }
  n->enfant.clear();
  delete(n);
  return;
}

int main(int argc, char *argv[]) {
  if (argc != 2) return 0;
  clock_t start = clock(); clock_t running = clock();
  vector<vector<uint>> idx(10000); //Initalisation 
  for (int i = 0; i < 10000; ++i) {
    vector<uint> vc; //Reservation
    idx[i] = vc; //Affection
  }
  queue<node*> tn; // terminal nodes
  #ifdef _BTREE
    stx::btree_map<uint, node *> _rootChild;
  #elif defined(_ROBIN)
    tsl::robin_map<uint, node *> _rootChild;
  #elif defined(_HOPSCOTCH)
    tsl::hopscotch_map<uint, node *> _rootChild;
  #else
    map<uint, node *> _rootChild;
  #endif

  vector<uint> tmp;
  concept2 superconcept(0, 0, 0, tmp);
  uint gCid = 0; uint gItems = 0;

#ifdef _VECTOR
  vector<concept2> fCI2(1,superconcept); //one space with value.
  ++gCid;
#else //très lent (map de la stl)
  map<uint, concept2> fCI2;
  fCI2.emplace(gCid++, superconcept);
#endif
  node **li = (node **)malloc(fCI2.size() * sizeof(node *));
  uint allocated_memory = 0; uint allocated_block = 0;
  printf("Initialisation en %0.4f ms\n", (clock() - start) / (double)CLOCKS_PER_SEC * 1000);

  FILE *f; uint row = 0;
  char s[10000];
  f = fopen(argv[1], "r");
  while (fgets(s, 10000, f) != NULL) {
    char *pch = strtok(s, " ");
    node *root = new node(_rootChild); //attention
    gItems = 0; //compteur d'items dans l'itemset pour optimizer le vecteur.
    uint requested_memory = (uint)fCI2.size()*sizeof(node *);
    //vector<node*> li(fCI2.size(), NULL); //v2.1f
    //node **li = new node*[fCI2.size()](); //v2.1g rev1
    //node **li = (node **)malloc(fCI2.size()*sizeof(node *)); //v2.1g rev2
    li = (node **)realloc(li, requested_memory);
    for (uint i = 0; i < allocated_block; ++i) {
      if (li + i != NULL)
        *(li + i) = NULL;
        //memset(li + i, 0, sizeof(node *));
    }
    memset(li + allocated_block, 0, requested_memory - allocated_memory);
    allocated_memory = requested_memory;
    allocated_block = (uint)fCI2.size();

    while (pch != 0) {
      ++gItems;
      uint item = atol(pch);
      //****auto it0 = idx.find(item);
      //---- mise a jour du superconcept (bottom) et de son index ---- start
      //****if (it0 == idx.end()) {
      if (idx[item].size() == 0) {
        vector<uint> v(1, 0); //reserve one space with value 0.
        idx[item] = v;
        if (fCI2[0].supp > 0) {
          concept2 c(gCid, fCI2[0].supp, fCI2[0].size); //concept2 c(gCid, fCI2[0].supp, fCI2[0].size, fCI2[0].itemset);
#ifdef _VECTOR
          for (uint t = 0; t < fCI2[0].itemset.size(); ++t) {
            idx[fCI2[0].itemset[t]].push_back(gCid);
          }
          fCI2.push_back(c); ++gCid;
#else
          fCI2.emplace_hint(fCI2.end(), gCid++, c);
#endif
          fCI2[0].supp = 0;
        }
        fCI2[0].itemset.push_back(item);
        fCI2[0].size = fCI2[0].size + 1;
      }
      //---- mise a jour du superconcept (bottom) et de son index ---- fin
      for (uint idConcept : idx[item]) { //tant qu'il y a des concepts (pour un item).
        concept2 *e = &fCI2.at(idConcept); //auto e = fCI2.at(idConcept); //e est un concept fCI2[idConcept]
        node *lin; //lin =est= last item node
        if (li[idConcept] != 0) { //v2.1g rev2
          lin = li[idConcept]; //v2.1g rev2
        //****auto it1 = li.find(idConcept); //li with B+tree
        //****if (it1 != li.end()) {
        //****  lin = it1->second;
        } else {
          lin = root; // root node
        }
        //------------------------------------------------------- DEBUT ---- UPDATE INTERSECTION
        node *n;
	      auto *_child = &lin->enfant;  //un petit truc pour les différentes declarations.  :)
        auto it2 = _child->find(item);
        if (it2 == _child->end()) {
          #ifdef _BTREE
            stx::btree_map<uint, node *> _enfant;
          #elif defined(_ROBIN)
	          tsl::robin_map<uint, node *> _enfant;
          #elif defined(_HOPSCOTCH)
	          tsl::hopscotch_map<uint, node *> _enfant;
          #else
            map<uint, node *> _enfant;
          #endif
          n = new node(_enfant);
          n->key = item;
          n->Cid = e->id;
          n->parent = lin;
          n->depth = lin->depth + 1;
          n->nb_ref = 0;
          #ifdef _BTREE
            lin->enfant.insert2(lin->enfant.end(), item, n); //auto itX = 
          #elif defined(_ROBIN)
	          lin->enfant.insert(lin->enfant.end(), std::make_pair(item, n)); //auto itX = 
          #elif defined(_HOPSCOTCH)
	          lin->enfant.insert(lin->enfant.end(), std::make_pair(item, n));
          #else
            lin->enfant.emplace_hint(lin->enfant.end(), item, n);
          #endif
          tn.push(n);
        } else {
          n = it2->second;
          concept2 *min = &fCI2.at(n->Cid);
          if (min->supp < e->supp) {
            n->Cid = e->id;
          }
        }
        ++n->nb_ref;
        if (lin->nb_ref > 0)
          --lin->nb_ref;
        li[e->id] = n;
        //bst_insert(bst, e->id); ++bst_size;
        //------------------------------------------------------ FIN ---- UPDATE INTERSECTION
      }
      pch = strtok(0, " ");
    }
    //free(li); //li.clear(); // delete[] li;
    while (!tn.empty()) {
      node *n = tn.front();
      if (n->nb_ref > 0) {
        uint newSupp = 1 + fCI2[n->Cid].supp;
#ifdef _VECTOR
        if (fCI2[n->Cid].size == n->depth) {
          fCI2[n->Cid].supp = newSupp;
        }
#else
        auto it5 = fCI2.find(n->Cid); // e est it5-second
        if (it5 != fCI2.end()) {
          if (it5->second.size == n->depth) {
            ++it5->second.supp;  // need fix ici pour concept id 0
          }
#endif
          else {
            //vector<uint> _itemset; //= path(n);
            //_itemset.reserve(gItems);
            ushort u=0;
            while (n->parent != NULL) {
              ++u; //size of itemset
              idx[n->key].push_back(gCid); //index update
           //   _itemset.push_back(n->key);
              n = n->parent;
            }
            //sort(_itemset.begin(), _itemset.end());
#ifdef _VECTOR
            concept2 C2(gCid, newSupp, u); //concept2 C2(gCid, newSupp, u, _itemset);
#else
            concept2 C2(gCid, it5->second.supp + 1, _itemset);
#endif
#ifdef _VECTOR
            fCI2.push_back(C2); ++gCid;
#else
            fCI2.emplace_hint(fCI2.end(), gCid++, C2);
          }
#endif
        }
      }
      tn.pop();
    }
    ++row;
    #ifdef DEBUG
    if ((row % 1000 == 0 && row < 10001) || row % 10000 == 0) {
      printf("elapsed time between checkpoint %0.2f ms, ", (clock() - running) / (double)CLOCKS_PER_SEC * 1000);
      running = clock();
      cout << row << " rows processed, idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
    }
    #endif
    freeNode(root);
  }
  fclose(f);
  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);
  cout  << row << " rows processed, idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
#ifdef DEBUG
  uint nb[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
  for (uint n = 0; n < fCI2.size(); ++n) {
    uint y = fCI2.at(n).supp;
    if (y < 10) {
      ++nb[y];
    }
    else {
      ++nb[10];
    }
  }
  for (uint n = 0; n < 11; ++n) { cout << n << "->" << nb[n] << endl; }
#endif
//#if _DEBUG
//  wf(fCI2, "out.txt");
//#endif
  fCI2.clear(); free(li); //idx.clear();
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX info;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *) &info, sizeof(info));
  std::cout << "WorkingSet " << info.WorkingSetSize/1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize/1024 << "K, PrivateSet " << info.PrivateUsage/1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
