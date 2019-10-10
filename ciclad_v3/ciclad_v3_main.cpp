// version 3.0a 2019-09-30 (insert, clean C++ template, stdin/stream)
// version 3.0b 2019-10-04 (remove)

#include "ciclad_v3_impl.h"   //uint, ushort, node3, concept3, freenode3()

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 1) return 0;
  clock_t start = clock(); clock_t running = clock();
  vector<vector<uint>> idx(10001); //Initalisation de index inversé
  for (int i = 0; i < 10001; ++i) {
    vector<uint> vc; //Reservation
    idx[i] = vc; //Affection
  }
  queue<node3 *> tn; // terminal nodes
  tlx::btree_map<uint, node3 *> _rootChild; //stx is the fastest
  vector<uint> tmp;
  concept3 superconcept(0, 0, 0, 0, tmp);
  uint gCid = 0;

  vector<concept3> fCI2(1, superconcept); //one space with value.
  ++gCid;

  Stats stats = Stats(); // garde les stats

  //node3 **li = (node3 **)malloc(fCI2.size() * sizeof(node3 *));
  //uint allocated_memory = 0; uint allocated_block = 0;
  double ms = 0;
  char s[10000]; char t[4] = { 0 }; char ss[10000];

  printf("Initialisation en %0.4f ms\n", (clock() - start) / (double)CLOCKS_PER_SEC * 1000);
  
  while (fgets(s, 10000, stdin) != NULL) {
    strncpy(t, s, 3);
    
    if (strcmp(t, "add") == 0 ) {     
      strcpy(ss, s + 4);
      ms = add(ss, tn, idx, _rootChild, fCI2, &gCid);
      stats.insert(&ms); //row++ dans la fonction;
    } else if (strcmp(t, "del") == 0) {
      strcpy(ss, s + 4);
      ms = del(ss, tn, idx, _rootChild, fCI2, &gCid);
      stats.remove(&ms);
    }

#ifdef DEBUG
      if ((row % 1000 == 0 && row < 10001) || row % 10000 == 0) {
        printf("elapsed time between checkpoint %0.2f ms, ", (clock() - running) / (double)CLOCKS_PER_SEC * 1000);
          running = clock();
          cout << row << " rows processed, idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
      }
#endif
    //freeNode3(root);
  }

  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);
  cout << stats.rows_inserted << " rows inserted, " << stats.rows_removed << " rows removed , idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
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
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  std::cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
