// Moment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "../generic_Transaction.h"
#include "CloStream.h"

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

  const uint32_t MAX_ATTRIBUTES = 1001;
  std::map<uint32_t, CloStreamCI*> tableClosed = std::map<uint32_t, CloStreamCI*>();
  std::map<uint32_t, std::vector<uint32_t>> cidListMap = std::map<uint32_t, std::vector<uint32_t>>();
  CloStreamCI emptySet = CloStreamCI();
  emptySet.support = 0;
  emptySet.itemset = new std::vector<uint32_t>();
  // add the empty set in the list of closed sets
  tableClosed.emplace(0, &emptySet);

  for (uint32_t i = 0; i != MAX_ATTRIBUTES; ++i) {
    cidListMap.emplace(i, std::vector<uint32_t>());
  }

  char s[10000];
  uint32_t i = 0;
  while (fgets(s, 10000, stdin) != NULL) {
    char *pch = strtok(s, " ");
    //if (i > 8123) break;
    if (0 != window_size && i >= window_size) {
      Transaction<uint32_t> old_transaction = window.front();
      removeOldTransaction(old_transaction.data(), &cidListMap, &tableClosed);
      window.pop();
    }
    Transaction<uint32_t> new_transaction = Transaction<uint32_t>(pch, " ", 0);
    if (i % 500 == 0) {
      std::cout << i << " transaction(s) processed" << std::endl;
    }
    processNewTransaction(new_transaction.data(), &cidListMap, &tableClosed);
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
  //std::cout << CLOSED_ITEMSETS.size() << std::endl;
  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);


#ifdef _WIN32
  //PROCESS_MEMORY_COUNTERS_EX info;
  //GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  //std::cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
