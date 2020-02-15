#include "../generic_Transaction.h"
#include "NewMoment.h"

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

uint32_t NEWCET_NODE_ID = 0;
std::map<uint32_t, std::vector<std::vector<bool>*>*> TRX_INDEX;
std::map<uint32_t, CETNode*> CLOSED_ITEMSETS;

uint32_t CET_NODE_ID = 0;
uint32_t NBR_NODES = 0;
uint32_t NBR_CLOSED_NODES = 0;

int main(int argc, char *argv[]) {
  if (argc != 5) return 0;
  clock_t start = clock(); clock_t running = clock();
  std::queue<Transaction<uint32_t>> window;
  const uint32_t window_size = strtoul(argv[1], 0, 10);//1500
  const uint32_t MAX_ATTRIBUTES = strtoul(argv[2], 0, 10);//100001
  const uint32_t minsup = strtoul(argv[3], 0, 10);//1
  const uint32_t BLOCK_SIZE = strtoul(argv[4], 0, 10);//1

  CETNode ROOT = CETNode();
  ROOT.children = new std::map<uint32_t, CETNode*>();
  ROOT.itemset = new std::vector<uint32_t>();
  ROOT.tidsum = 0;
  std::map<long, std::vector<std::vector<CETNode*>*>*> EQ_TABLE = std::map<long, std::vector<std::vector<CETNode*>*>*>();
  
  //const uint32_t MAX_ATTRIBUTES = 1001;
  //initialiser l'arbre (autant de noeuds de d'items)
  //ou on peut le faire a chaque trx ? si nouvel item, on rajoute l'item dans l'arbre ?
  for (int i = 0; i != MAX_ATTRIBUTES; ++i) {
    CETNode* atom = new CETNode();
    ROOT.children->emplace(i, atom);
    atom->parent = &ROOT;
    atom->item = i;
    atom->itemset = new std::vector<uint32_t>();
    atom->itemset->push_back(i);
    atom->type = 0;//
    //atom->tidlist = new std::vector<uint32_t>();// [0];
    atom->tidsum = 0;
    atom->id = ++NEWCET_NODE_ID;
    NBR_NODES += 1;
  }
 
  uint32_t trx_shift = 0;
  char s[10000];
  uint32_t i = 0;
  while (fgets(s, 10000, stdin) != NULL) {
    char *pch = strtok(s, " ");
    //if (i > 9998) break;
    Transaction<uint32_t> new_transaction = Transaction<uint32_t>(pch, " ", 0);
    if (0 != window_size && i >= window_size) {
      trx_shift += 1;
      if (trx_shift % BLOCK_SIZE == 0) {
        trx_shift = 0;
      }
    }

    update_index_sliding_window(new_transaction.data(), i, trx_shift, window_size, MAX_ATTRIBUTES, BLOCK_SIZE);
    if (0 != window_size && i >= window_size) {
      //delete
      Transaction<uint32_t> old_transaction = window.front();
      const uint32_t trx_shift_right = trx_shift == 0 ? 0 : trx_shift - 1;
      Delete(1 + (i - window_size), old_transaction.data(), minsup, &ROOT, window_size, trx_shift, trx_shift_right, &EQ_TABLE, BLOCK_SIZE);
      window.pop();
    }
    
    if (i % 50 == 0) {
      std::cout << i << " transaction(s) processed" << std::endl;
    }
    Append(i + 1, new_transaction.data(), minsup, &ROOT, window_size, trx_shift, trx_shift, &EQ_TABLE, BLOCK_SIZE);
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
  std::cout << NBR_CLOSED_NODES << std::endl;
  std::cout << NBR_NODES << std::endl;
  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);

  {
    prune_children(&ROOT, 0, &EQ_TABLE);
    delete ROOT.children;
    delete ROOT.itemset;
    delete ROOT.tidlist;
    //nettoyer, children, itemset
    //y aller directement DFS
  }

  //nettoyage de la map (TODO: put this in a function)
  {
    std::map<long, std::vector<std::vector<CETNode*>*>*>::iterator it = EQ_TABLE.begin();
    for (; it != EQ_TABLE.end(); ++it) {
      std::vector<std::vector<CETNode*>*>* eq_class_stratified = it->second;
      std::vector<std::vector<CETNode*>*>::iterator eq_class_stratified_it = eq_class_stratified->begin();
      for (; eq_class_stratified_it != eq_class_stratified->end(); ++eq_class_stratified_it) {
        std::vector<CETNode*>* eq_class = *eq_class_stratified_it;
        delete eq_class;//Not necessary to clean referenced node pointers, they are cleaned below with the tree
      }
      delete eq_class_stratified;
    }
  }

  //nettoyage de l'arbre (TODO: put this in a function)
  //NOTA: peut etre prune_children pourrait faire l'affaire ici !!
  {
    //nettoyer, children, itemset
    //y aller directement DFS
  }

#ifdef _WIN32
  //PROCESS_MEMORY_COUNTERS_EX info;
  //GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  //std::cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}
