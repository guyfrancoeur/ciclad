//
//  Copyright © 2018 [CICLAD TEAM]
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.If not, see <http://www.gnu.org/licenses/>.
//

#include "Ciclad.h"
#include "TestsCiclad.h"


using namespace std;

uint32_t __maxItem;
size_t numberOfCI = 1;

int main(int argc, char * argv[]) {
  if (true) {
    const uint32_t nbr_items = strtoul(argv[2], 0, 10);
    //Attention : il faudrait aussi chercher le errno
    if (nbr_items == 0 || nbr_items == ULONG_MAX || nbr_items == ULLONG_MAX) {
      std::cout << "Cannot read number of items " << std::endl;
      exit(1);
    }
    const uint32_t window_size = strtoul(argv[3], 0, 10);
    /*if (window_size == 0 || window_size == ULONG_MAX || window_size == ULLONG_MAX) {
      std::cout << "Cannot read window size " << std::endl;
      exit(1);
    }*/

    //item du "bottom"
    const size_t maxItem = nbr_items;// MAX_ITEM;
    std::cout << "nbr items " << maxItem << ", window size " << window_size << std::endl;
    const size_t windowSize = window_size;// WINDOW_SIZE;

    const uint32_t nbrCI = start_ciclad(argv[1], windowSize, maxItem);
  }
  else {
    test_output();
  }
  
  return 0;
}

uint32_t start_ciclad(char* const _fileSource, const uint32_t _windowSize, const uint32_t _maxItem) {
  std::queue<TRANSACTION> transactionStream, window;
  //readfile(argv[1], transactionStream);
  readfile(_fileSource, transactionStream);

  __maxItem = _maxItem;

  concept bottomConcept;
  bottomConcept.id = 0;
  bottomConcept.lastitem = 0;
  bottomConcept.size = _maxItem;
  bottomConcept.support = 0;

  numberOfCI = 0;
  trxid = 0;

#ifdef REUSE_OBSOLETE
  available_positions_for_new_cis = (std::queue<size_t>**)malloc(_maxItem * sizeof(std::queue<size_t>*));
  for (uint32_t i = 0; i < _maxItem; ++i) {
    available_positions_for_new_cis[i] = (std::queue<size_t>*)new std::queue<size_t>();
  }
#endif

  vector<concept*> conceptContainer;
  conceptContainer.push_back(&bottomConcept);
  vector<vector<concept*>>index;
  {
    for (size_t i = 0; i < _maxItem; ++i) {
      vector<concept*> tmp;
      tmp.push_back(conceptContainer[0]);
      index.push_back(tmp);
    }
  }

  clock_t begin;
  clock_t end;

  begin = clock();

  int tran = 0;
  for (size_t tID = 0; tID < _windowSize; ++tID) {
    TRANSACTION current = transactionStream.front();
    window.push(current);
    updateCicladAdd(&current, &index, &conceptContainer);
    transactionStream.pop();
    trxid += 1;
  }
  std::cout << conceptContainer.size() << " " << numberOfCI << endl;

  while (!transactionStream.empty()) {
    //if not last in stream
    if (0 != _windowSize) {
      updateCicladRmv(&window.front(), &index, &conceptContainer);
      window.pop();
      window.push(transactionStream.front());
    }
    updateCicladAdd(&transactionStream.front(), &index, &conceptContainer);

    transactionStream.pop();
    //TODO: this should be an argument
    if (trxid % 1000 == 0) {
      std::cout << "trx " << trxid << std::endl;
    }
    trxid += 1;
  }
  //cleaning
  while (!window.empty()) {
    free((&window.front())->itemset);
    window.pop();
  }

  end = clock();
  std::printf("processed transactions in %0.4f ms\n", (end - begin) / (double)CLOCKS_PER_SEC * 1000);
  std::cout << " number of ci : " << numberOfCI << "(" << conceptContainer.size() << ")" << flush;

#ifdef REUSE_OBSOLETE
  for (uint32_t i = 0; i < _maxItem; ++i) {
    delete available_positions_for_new_cis[i];
  }
  free(available_positions_for_new_cis);
#endif

  std::map<uint32_t, vector<uint32_t>> actual_itemsets;
  std::map<uint32_t, concept*> rebuilt_cis;
  std::map<uint32_t, vector<uint32_t>>::iterator ref_ci;
  for (std::vector<std::vector<concept*>>::iterator iter = index.begin(); iter != index.end(); ++iter) {
    std::vector<concept*> cid_list = *iter;
    for (std::vector<concept*>::iterator iter2 = cid_list.begin(); iter2 != cid_list.end(); ++iter2) {
      concept* ci = *iter2;
      if (!ci || !ci->id) continue;
      if ((ref_ci = actual_itemsets.find(ci->id)) != actual_itemsets.end()) {
        actual_itemsets.at(ci->id).emplace_back(iter - index.begin());
      }
      else {
        vector<uint32_t> first_item;
        first_item.emplace_back(iter - index.begin());
        actual_itemsets.emplace(ci->id, first_item);
      }
    }
  }

  /*for (std::map<uint32_t, std::vector<uint32_t>>::iterator iter = actual_itemsets.begin(); iter != actual_itemsets.end(); ++iter) {
    uint32_t cid = iter->first;
    vector<uint32_t> itemset = iter->second;
    {
      printf(" CI #%d{", cid);
      vector<uint32_t>::iterator newciit = itemset.begin();
      for (; newciit != itemset.end(); ++newciit) {
        printf("%d,", *newciit);
      }
      printf("} ");
    }
  }*/

  /*
  for (std::vector<concept*>::iterator ci_it = conceptContainer.begin(); ci_it != conceptContainer.end(); ++ci_it) {
    concept* ci_to_clean = *ci_it;
    if (ci_to_clean) {
      free(ci_to_clean->positionsInIndex);
      free(ci_to_clean);
    }
  }*/

#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX info;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return numberOfCI;
}


int readfile(char *fn, std::queue<TRANSACTION> &T) {
  ifstream inFile(fn);
  if (!inFile) {
    cerr << "cannot open INPUT file!" << endl;
    exit(1);
  }

  while (!inFile.eof()) {
    vector<uint32_t>* items = new vector<uint32_t>();
    items->push_back(0);
    string line;
    string token;
    std::getline(inFile, line);
    istringstream iss(line);
    while (getline(iss, token, ' ')) {
      items->push_back(stol(token));
    }
    TRANSACTION trx;
    trx.itemset = &(*(items->begin()));
    *(trx.itemset) = items->size() - 1;
    T.push(trx);
  }
  return 0;
}