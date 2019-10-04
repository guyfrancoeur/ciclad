// version 3.0a 2019-09-30 (insert, clean C++ template, stdin/stream)
// version 3.0b 2019-10-04 (remove)
// voir Makefile pour build, test, valgrind (memory leak), time (memory usage, execution time)
#include <cstdio>   //fopen, printf
#include <cstdlib>  //atol
#include <queue>    //sliding window
#include <cstring>  //strtok
#include <ctime>    //clock_t
#include <iostream> //cout
#include "isci.h"   //cmdline, uint, ushort, node3, concept3

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
  queue<node3 *> tn; // terminal nodes
  stx::btree_map<uint, node3 *> _rootChild; //stx is the fastest
  vector<uint> tmp;
  concept3 superconcept(0, 0, 0, tmp);
  uint gCid = 0;

  vector<concept3> fCI2(1,superconcept); //one space with value.
  ++gCid;

  //node3 **li = (node3 **)malloc(fCI2.size() * sizeof(node3 *));
  //uint allocated_memory = 0; uint allocated_block = 0;
  printf("Initialisation en %0.4f ms\n", (clock() - start) / (double)CLOCKS_PER_SEC * 1000);
  double ms=0;
  uint row = 0; uint a=0, d=0;
  char s[10000];
  while (fgets(s, 10000, stdin) != NULL) {
    char *pch = strtok(s, " ");
    if (strcmp(pch,"add") == 0 {
      ms = add(s, tn, idx, _rootchild, &superconcept, fCI2, &gCid);
      a++;
    }
    if (strcmp(pch,"del") == 0 {
      ms = del(s, tn, idx, _rootchild, &superconcept, fCI2, &gCid);
      d++;
    }
    ++row;

    #ifdef DEBUG
    if ((row % 1000 == 0 && row < 10001) || row % 10000 == 0) {
      printf("elapsed time between checkpoint %0.2f ms, ", (clock() - running) / (double)CLOCKS_PER_SEC * 1000);
      running = clock();
      cout << row << " rows processed, idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
    }
    #endif
    freeNode3(root);
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
  fCI2.clear();
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX info;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *) &info, sizeof(info));
  std::cout << "WorkingSet " << info.WorkingSetSize/1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize/1024 << "K, PrivateSet " << info.PrivateUsage/1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
