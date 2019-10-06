#ifndef CLOSTREAM_ALG_H
#define CLOSTREAM_ALG_H

#include "../CommonUtility/Utility.h"

#include <set>
#include <map>
#include <vector>

struct CloStreamCI {
  uint32_t support;
  uint32_t id;
  std::vector<uint32_t>* itemset;
  std::vector<uint32_t>* positions_in_lists;
};
class LexicographicItemsetComparator {
public:
  bool operator() (CloStreamCI* const _lhs, CloStreamCI* const _rhs) const
  {
    const uint32_t smaller = _lhs->itemset->size() > _rhs->itemset->size() ? _rhs->itemset->size() : _lhs->itemset->size();
    for (int i = 0; i != smaller; ++i) {
      if (_lhs->itemset->at(i) > _rhs->itemset->at(i)) return false;
      else if (_lhs->itemset->at(i) < _rhs->itemset->at(i)) return true;
    }
    if (_lhs->itemset->size() > _rhs->itemset->size()) {
      return false;
    }
    else if (_lhs->itemset->size() < _rhs->itemset->size()) {
      return true;
    }
    return false;
  }
};

std::set<uint32_t>* compute_union_ideals(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap);

std::map<CloStreamCI*, uint32_t>::iterator* locate_intersection_class(CloStreamCI* const _candidateGenitor, CloStreamCI* _intersection, std::map<CloStreamCI*, uint32_t, LexicographicItemsetComparator>* const _tableTemp);

void processNewTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed);

void removeOldTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed);

#endif
