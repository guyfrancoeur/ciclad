#ifndef CFISTREAM_ALG_H
#define CFISTREAM_ALG_H

#include "../CommonUtility/Utility.h"

#include <map>
#include <vector>
#include <iostream>

struct DUINode {
  uint32_t item;
  uint32_t count;
  uint32_t support;
  uint32_t positionInStrate = 0;
  bool flaged_for_removal = false;
  DUINode* parent;
  std::vector<uint32_t>* itemset;
  std::map<uint32_t, DUINode*>* children;
};
class CFIStreamLexicographicItemsetComparator {
public:
  bool operator() (DUINode* const _lhs, DUINode* const _rhs) const {
    return LexicographicItemsetComparator().compare(_lhs->itemset, _rhs->itemset);
  }
};


void Addition(DUINode* const _root, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _new_cis);

void Suppression(DUINode* const _root, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _new_cis);

void enumerate_supersets(std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _superset_cis);

void enumerate_subsets(DUINode* const _ref, const uint8_t _action, const bool _strict);

DUINode* is_in_tree(std::vector<uint32_t>* const _transaction, DUINode* const _root);

DUINode* count(std::vector<uint32_t>* const _transaction, DUINode* const _node);

uint32_t support(std::vector<uint32_t>* const _transaction, DUINode* const _node);

void insert_in_tree(DUINode* const _node, DUINode* const _root);

void enumerate_subsets_stratified(std::vector<uint32_t>* const _transaction, std::vector<std::vector<DUINode*>>* const _all_subsets_stratified);

void remove_from_tree(DUINode* const _node, DUINode* const _root);

void Suppression(DUINode* const _node, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _obs_cis);

#endif
