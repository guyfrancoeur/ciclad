#ifndef CLOSTREAM_ALG_H
#define CLOSTREAM_ALG_H

#include "../CommonUtility/Utility.h"

#include <set>
#include <map>
#include <vector>
#include <queue>
#include <iostream>

struct CloStreamCI {
  uint32_t support;
  uint32_t id;
  std::vector<uint32_t>* itemset;
  std::vector<uint32_t>* positions_in_lists;

  bool operator<(CloStreamCI*& const _rhs) {
    return LexicographicItemsetComparator().compare(this->itemset, _rhs->itemset);
  };
};
class CloStreamLexicographicItemsetComparator {
public:
  bool operator() (CloStreamCI* const _lhs, CloStreamCI* const _rhs) const{
    return LexicographicItemsetComparator().compare(_lhs->itemset, _rhs->itemset);
  };
};

std::set<uint32_t>* compute_union_ideals(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap);

uint32_t locate_intersection_class(CloStreamCI* const _candidateGenitor, CloStreamCI* _intersection, std::vector<std::pair<CloStreamCI*, uint32_t>>* const _tableTemp);

void processNewTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed);

void removeOldTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed);

const uint32_t ERROR_TRYING_TO_REMOVE_CI_WITH_NO_CI_LIST = 0x01;

#endif
