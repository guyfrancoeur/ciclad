#include "../generic_Transaction.h"
#include "CFIStream.h"

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

uint32_t NBR_CLOSED_NODES = 0;
//std::map<uint32_t, DUINode*> CLOSED_ITEMSETS;

std::map<uint32_t, std::vector<DUINode*>> HEADER_STRATE;
std::map<uint32_t, DUINode*> HEADER_MIN;


std::map<uint32_t, std::vector<std::vector<uint32_t>>> enumerations;
uint32_t TOTAL_CANDIDATES = 0;

bool X_close = true;//Il semble que cela serve uniquement a s arreter lorsque la transaction est deja un CI ???
std::vector<uint32_t>* X_0 = 0;

int main(int argc, char *argv[]) {
  if (argc != 4) return 0;
  clock_t start = clock(); clock_t running = clock();
  std::queue<Transaction<uint32_t>> window;
  const uint32_t window_size = strtoul(argv[1], 0, 10);//1500
  const uint32_t MAX_ATTRIBUTES = strtoul(argv[2], 0, 10);//100001
  const uint32_t minsup = strtoul(argv[3], 0, 10);//1

  DUINode ROOT = DUINode();
  ROOT.children = new std::map<uint32_t, DUINode*>();
  ROOT.itemset = new std::vector<uint32_t>();
  //const uint32_t MAX_ATTRIBUTES = 1001;

  char s[10000];
  uint32_t i = 0;
  while (fgets(s, 10000, stdin) != NULL) {
    char *pch = strtok(s, " ");
    if (0 != window_size && i >= window_size) {
      //delete
      Transaction<uint32_t> old_transaction = window.front();
      std::cout << "removing" << std::endl;
      std::vector<DUINode*> obs_cis = std::vector<DUINode*>();
      Suppression(&ROOT, old_transaction.data(), &obs_cis);
      {
        std::cout << obs_cis.size() << " to remove" << std::endl;
        std::sort(obs_cis.begin(), obs_cis.end(), CFIStreamLexicographicItemsetComparator());
        //placer tous les noeuds dans new_cis
        std::vector<DUINode*>::iterator it = obs_cis.begin();
        for (; it != obs_cis.end(); ++it) {
          DUINode* const node = *it;
          const uint32_t len = node->itemset->size();
          if (HEADER_STRATE.find(len) ==  HEADER_STRATE.end()) {
            //erreur
            exit(1);
          }
          std::vector<DUINode*>* const strate = &HEADER_STRATE.find(len)->second;
          DUINode* const n = strate->back();
          strate->at(node->positionInStrate) = n;
          strate->pop_back();
          n->positionInStrate = node->positionInStrate;
          remove_from_tree(node, &ROOT);
          NBR_CLOSED_NODES -= 1;
          //System.out.println("removed " + Arrays.toString(node.itemset) + " " + node.support + " vs " + node.count);
        }
      }
      window.pop();
    }
    Transaction<uint32_t> new_transaction = Transaction<uint32_t>(pch, " ", 0);
    if (i % 500 == 0) {
      std::cout << i << " transaction(s) processed" << std::endl;
    }

    //This should go to a separate function
    {
      std::vector<DUINode*> new_cis = std::vector<DUINode*>();
      enumerations.clear();
      TOTAL_CANDIDATES = 0;
      X_0 = 0;
      X_close = true;
      //std::cout << "starting addition " << std::endl;
      Addition(&ROOT, new_transaction.data(), &new_cis);
      {
        std::sort(new_cis.begin(), new_cis.end(), CFIStreamLexicographicItemsetComparator());
        //placer tous les noeuds dans new_cis
        std::vector<DUINode*>::iterator it = new_cis.begin();
        for(; it != new_cis.end(); ++it){
          DUINode* const node = *it;
          //inserer dans l'arbre     
          //std::cout << "inserting " << std::endl;
          //print_array(node->itemset);
          insert_in_tree(node, &ROOT);
          NBR_CLOSED_NODES += 1;
          //ensuite, rajouter dans les listes par taille
          const uint32_t len = node->itemset->size();
          if (HEADER_STRATE.find(len) == HEADER_STRATE.end()) {
            HEADER_STRATE.emplace(len, std::vector<DUINode*>());
          }
          node->positionInStrate = (&HEADER_STRATE.find(len)->second)->size();
          (&HEADER_STRATE.find(len)->second)->push_back(node);
        }
      }
    }
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

  std::cout << NBR_CLOSED_NODES << std::endl;

  //coder le cleanup du DUITree
  std::map<uint32_t, std::vector<DUINode*>>::iterator it = HEADER_STRATE.begin();
  for (; it != HEADER_STRATE.end(); ++it) {
    std::vector<DUINode*>* const cis = &it->second;
    std::vector<DUINode*>::iterator it2 = cis->begin();
    for (; it2 != cis->end(); ++it2) {
      //System.out.println("" + Arrays.toString(n.itemset) + " " + n.support);
    }
  }
  //System.out.println("" + NBR_CLOSED_NODES);
  printf("Stream completed in %0.2f sec, ", (clock() - start) / (double)CLOCKS_PER_SEC);
#ifdef _WIN32
  //PROCESS_MEMORY_COUNTERS_EX info;
  //GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&info, sizeof(info));
  //std::cout << "WorkingSet " << info.WorkingSetSize / 1024 << "K, PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << "K, PrivateSet " << info.PrivateUsage / 1024 << "K" << endl;
#endif
  return EXIT_SUCCESS;
}