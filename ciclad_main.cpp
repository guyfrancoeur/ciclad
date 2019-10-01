// version 3.0a 2019-09-30 (add, clean C++ template, stdin/stream)
// version 3.0b 2019-09-xx (remove)
// voir Makefile pour build, test, valgrind (memory leak), time (memory usage, execution time)
//PREPROCESSORS (also for isci.h) to be fixed... sans les MACROS! utiliser -D a la compilation !
#define _BTREE  // _BTREE | _ROBIN | _HOPSCOTCH | _STLMAP_a // structure pour la trie
#define _VECTOR // _VECTOR | _STLMAP_b                      // structure pour les concepts

#include <cstdio>   //fopen, printf
#include <cstdlib>  //atol
#include <queue>    //sliding window
#include <cstring>  //strtok
#include <ctime>    //clock_t
#include <iostream> //cout
#include "isci.h"   //cmdline, uint (TODO a fix needed for template only)

#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
#endif

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 1) return 0;
  clock_t start = clock(); clock_t running = clock();
  vector<vector<uint>> idx(10000); //Initalisation de index inversé
  for (int i = 0; i < 10000; ++i) {
    vector<uint> vc; //Reservation
    idx[i] = vc; //Affection
  }
  queue<node*> tn; // terminal nodes
  stx::btree_map<uint, node *> _rootChild; //stx is the fastest
  vector<uint> tmp;
  concept2 superconcept(0, 0, 0, tmp);
  uint gCid = 0;

  vector<concept2> fCI2(1,superconcept); //one space with value.
  ++gCid;

  node **li = (node **)malloc(fCI2.size() * sizeof(node *));
  uint allocated_memory = 0; uint allocated_block = 0;
  printf("Initialisation en %0.4f ms\n", (clock() - start) / (double)CLOCKS_PER_SEC * 1000);

  uint row = 0;
  char s[10000];
  while (fgets(s, 10000, stdin) != NULL) {
    // prototype add(char *s, auto tn, auto idx, auto _rootchild, auto superconcept, auto fCI2, auto li, unit gCid);
    // start add() --->
    node *root = new node(_rootChild); //attention
    uint requested_memory = (uint)fCI2.size()*sizeof(node *);
    li = (node **)realloc(li, requested_memory);
    for (uint i = 0; i < allocated_block; ++i) {
      if (li + i != NULL)
        *(li + i) = NULL;
    }
    memset(li + allocated_block, 0, requested_memory - allocated_memory);
    allocated_memory = requested_memory;
    allocated_block = (uint)fCI2.size();

    char *pch = strtok(s, " ");
    while (pch != 0) {
      uint item = atol(pch);
      //---- mise a jour du superconcept (bottom) et de son index ---- start
      if (idx[item].size() == 0) {
        vector<uint> v(1, 0); //reserve one space with value 0.
        idx[item] = v;
        if (fCI2[0].supp > 0) {
          concept2 c(gCid, fCI2[0].supp, fCI2[0].size); //concept2 c(gCid, fCI2[0].supp, fCI2[0].size, fCI2[0].itemset);
          for (uint t = 0; t < fCI2[0].itemset.size(); ++t) {
            idx[fCI2[0].itemset[t]].push_back(gCid);
          }
          fCI2.push_back(c); ++gCid;
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
        } else {
          lin = root; //root node
        }
        //------------------------------------------------------- DEBUT ---- UPDATE INTERSECTION
        node *n;
	      auto *_child = &lin->enfant;  //un petit truc pour les différentes declarations.  :)
        auto it2 = _child->find(item);
        if (it2 == _child->end()) {
          stx::btree_map<uint, node *> _enfant;
          n = new node(_enfant);
          n->key = item;
          n->Cid = e->id;
          n->parent = lin;
          n->depth = lin->depth + 1;
          n->nb_ref = 0;
          lin->enfant.insert2(lin->enfant.end(), item, n);
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
        //------------------------------------------------------ FIN ---- UPDATE INTERSECTION
      }
      pch = strtok(0, " ");
    }

    while (!tn.empty()) {
      node *n = tn.front();
      if (n->nb_ref > 0) {
        uint newSupp = 1 + fCI2[n->Cid].supp;

        if (fCI2[n->Cid].size == n->depth) {
          fCI2[n->Cid].supp = newSupp;
        } else {
          ushort u=0;
          while (n->parent != NULL) {
            ++u; //size of itemset
            idx[n->key].push_back(gCid); //index update
            n = n->parent;
          }
          concept2 C2(gCid, newSupp, u);
          fCI2.push_back(C2); ++gCid;
        }
      }
      tn.pop();
    }
    // end add() --->
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
  fCI2.clear(); free(li);
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX info;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *) &info, sizeof(info));
  std::cout << "WorkingSet " << info.WorkingSetSize/1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize/1024 << "K, PrivateSet " << info.PrivateUsage/1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
