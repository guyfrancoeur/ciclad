// version 3.0a 2019-09-30 (insert, clean C++ template, stdin/stream)
// version 3.0b 2019-10-04 (beta remove with leaks)
// version 3.0c 2020-07-03 (remove with cleanup function)

#include "ciclad_v3_impl.h"   //uint, ushort, node3, concept3, freenode3()
#include "../utility/usage.h"  //ram-cpu usage utility (Win32 or Linux)

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char *argv[]) {
#ifdef LINUX
  struct rusage ru;
#endif
  short verbose = 0; short END = 0;
  if (argc == 2) {
    if (strcmp(argv[1], "-v") == 0) { verbose = 1; }
  }
  else { std::cout << "verbose mode inactive." << endl; }
  clock_t start = clock(); clock_t running = clock();
  std::vector<vector<uint>> idx(10001); //Initalisation de index inversé
  for (int i = 0; i < 10001; ++i) {
    vector<uint> vc; //Reservation
    idx[i] = vc; //Affection
  }
  std::queue<node3 *> tn; // terminal nodes
  tlx::btree_map<uint, node3 *> _rootChild; //stx is the fastest
  std::vector<uint> tmp;
  concept3 superconcept(0, 0, /*0,*/ 0, tmp);
  uint gCid = 0;

  std::vector<concept3> fCI2(1, superconcept); //add one space with one value (CI).
  ++gCid;

  Stats stats = Stats(); // garde les stats

  //node3 **li = (node3 **)malloc(fCI2.size() * sizeof(node3 *));
  //uint allocated_memory = 0; uint allocated_block = 0;
  double ms = 0;
  char s[10000]; char t[4] = { 0 }; char ss[10000];

  printf("Initialisation en %0.4f ms\n", (clock() - start) / (double)CLOCKS_PER_SEC * 1000);

  while ((fgets(s, 10000, stdin) != NULL) && (END == 0)) {
    strncpy(t, s, 3);

    if (strcmp(t, "add") == 0) {
      strcpy(ss, s + 4);
      ms = add(ss, tn, idx, _rootChild, fCI2, &gCid);
#ifdef DEBUG
      stats.insert(&ms);
#endif
    }
    else if (strcmp(t, "del") == 0) {
      strcpy(ss, s + 4);
      ms = del(ss, tn, idx, _rootChild, fCI2, &gCid);
#ifdef DEBUG
      stats.remove(&ms);
#endif
      ms = cleanup(idx, fCI2);
    }
    else if (strcmp(t, "end") == 0) END = 1;
    ++stats.rows_processed;
    if (verbose) {
      if ((stats.rows_processed % 1000 == 0 && stats.rows_processed < 10001) || stats.rows_processed % 10000 == 0) {
        printf("time between checkpoint %0.2f ms, ", (clock() - running) / (double)CLOCKS_PER_SEC * 1000);
        running = clock();
        std::cout << stats.rows_processed << " rows processed, # CI:" << fCI2.size() << endl;
      }
    }
  }

  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);
  std::cout << stats.rows_inserted << " rows inserted, " << stats.rows_removed << " rows removed , idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
  if (verbose) {
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
    for (uint n = 0; n < 11; ++n) { std::cout << n << "->" << nb[n] << endl; }
  }
  fCI2.clear();
#ifdef _WIN32
  pu_ram();
#endif
#ifdef LINUX
  gu_all(&ru);
  pu_ram(ru);
#endif
  return EXIT_SUCCESS;
}
