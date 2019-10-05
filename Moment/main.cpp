// Moment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "../generic_Transaction.h"
#include "moment.h"

#include <cstdio>   //fopen, printf
#include <cstdlib>  //atol
#include <queue>    //sliding window
#include <cstring>  //strtok
#include <ctime>    //clock_t
#include <iostream> //cout
#include <map>
#include <vector>
#include <set>


#ifdef _WIN32
//#include <windows.h>
//#include <psapi.h>
#endif

int main(int argc, char *argv[]) {
  if (argc != 1) return 0;
  clock_t start = clock(); clock_t running = clock();
  std::queue<Transaction<uint32_t>> window;
  uint32_t window_size = 50;

  CETNode ROOT = CETNode();
  std::map<long, std::vector<std::vector<CETNode*>*>*> EQ_TABLE = std::map<long, std::vector<std::vector<CETNode*>*>*>();
  uint32_t minsup = 0;

  char s[10000];
  uint32_t i = 0;
  while (fgets(s, 10000, stdin) != NULL) {
    char *pch = strtok(s, " ");
    if (0 != window_size && i >= window_size) {
      //delete
      Transaction<uint32_t> old_transaction = window.front();
      //Deletion(1 + (i - window_size), old_transaction.data(), minsup, &ROOT, &EQ_TABLE);
      std::cout << "removed something " << std::endl;
      window.pop();
    }
    Transaction<uint32_t> new_transaction = Transaction<uint32_t>(pch, " ", 0);
    //add
    std::cout << "added something " << std::endl;
    //Addition(i + 1, new_transaction.data(), minsup, &ROOT, &EQ_TABLE);
    window.push(new_transaction);
    i += 1;

#ifdef DEBUG
      if ((row % 1000 == 0 && row < 10001) || row % 10000 == 0) {
        printf("elapsed time between checkpoint %0.2f ms, ", (clock() - running) / (double)CLOCKS_PER_SEC * 1000);
          running = clock();
          cout << row << " rows processed, idx size/capacity:" << idx.size() << "/" << idx.capacity() << ", # concept:" << fCI2.size() << endl;
      }
#endif
  }

  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);
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
#ifdef _WIN32
  //PROCESS_MEMORY_COUNTERS_EX info;
  //GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  //std::cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
