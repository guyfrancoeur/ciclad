#ifndef MOMENT_ALG_H
#define MOMENT_ALG_H

//#include "../CommonUtility/Utility.h"

#include <map>
#include <vector>
#include <algorithm>

struct CETNode {
  uint8_t type;
  uint32_t support;
  uint32_t item;
  uint32_t tidsum = 0;
  uint32_t id;

  long hash;
  long oldHash;

  std::vector<uint32_t>* itemset;
  std::map<uint32_t, CETNode*>* children;
  CETNode* parent;
  std::vector<uint32_t>* tidlist;
};

const uint8_t CLOSED_NODE = 0x04;
const uint8_t INTERMEDIATE_NODE = 0x02;
const uint8_t UNPROMISSING_GATEWAY_NODE = 0x01;
const uint8_t INFREQUENT_GATEWAY_NODE = 0x03;

void Explore(const uint32_t _tid, CETNode* const _node,  std::vector<uint32_t>* const _transaction, const uint32_t _minsupp, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _TABLE);

void Addition(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, const CETNode* _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);

void Deletion(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, const CETNode* _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);

std::vector<uint32_t>* left_check(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _TABLE);

void add_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void delete_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void prune_children(CETNode* const _node, const uint32_t _tid, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void remove_from_class(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void update_cetnode_in_hashmap(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);

//error codes
const uint32_t ERROR_INCORRECT_CREATION_OF_NODE = 0x01;//An invalid node has been created: its support is higher than at least one of its parents (i.e. violates downward closure property)
const uint32_t ERROR_COULD_NOT_FIND_CI_OLD_CLASS = 0x02;//An existing CI's class could not be found in hashtable: either its hash is invalid or the hashtable is not correct (i.e. integrity error)
const uint32_t ERROR_COULD_NOT_FIND_CI = 0x04;//An existing CI class could not be found in hashtable: either its hash is invalid or the hashtable is not correct (i.e. integrity error)
const uint32_t ERROR_DELETE_NODE_WITH_EMPTY_TIDSET = 0x08;//A node has an empty tidset: then it should not exist (i.e. integrity error)
const uint32_t ERROR_DELETE_NODE_WITH_HASH = 0x10;
const uint32_t ERROR_DELETE_NODE_CONTAINS_TRX_BUT_NOT_IN_TIDSET = 0x20;
const uint32_t ERROR_DELETE_REMOVING_INFREQUENT_CI = 0x40;//Removing some infrequent closed node

#endif
