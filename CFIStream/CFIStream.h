#ifndef CFISTREAM_ALG_H
#define CFISTREAM_ALG_H

#include "../CommonUtility/Utility.h"

#include <map>
#include <vector>

struct DUINode {
  uint32_t count;
  uint32_t support;
  std::vector<uint32_t>* itemset;
  std::map<uint32_t, DUINode*>* children;
};
class CFIStreamLexicographicItemsetComparator {
public:
  bool operator() (DUINode* const _lhs, DUINode* const _rhs) const {
    return LexicographicItemsetComparator().compare(_lhs->itemset, _rhs->itemset);
  }
};


std::map<uint32_t, std::vector<DUINode*>> HEADER_STRATE;
std::map<uint32_t, DUINode*> HEADER_MIN;


std::map<uint32_t, std::vector<std::vector<uint32_t>>> enumerations;
uint32_t TOTAL_CANDIDATES = 0;

const uint8_t INCREMENT = 0x01;
const uint8_t DECREMENT = 0x02;

bool X_close = true;//Il semble que cela serve uniquement a s arreter lorsque la transaction est deja un CI ???
std::vector<uint32_t>* X_0 = 0;

void Addition(DUINode* const _root, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _new_cis);

void enumerate_supersets(std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _superset_cis);

void enumerate_subsets(DUINode* const _ref, const uint8_t _action, const bool _strict);

DUINode* is_in_tree(std::vector<uint32_t>* const _transaction, DUINode* const _root);

DUINode* count(std::vector<uint32_t>* const _transaction, DUINode* const _node);

uint32_t support(std::vector<uint32_t>* const _transaction, DUINode* const _node);

void insert_in_tree(DUINode* const _node, DUINode* const _root);

#endif
