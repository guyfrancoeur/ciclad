#ifndef NEWMOMENT_ALG_H
#define NEWMOMENT_ALG_H

#include "../CommonUtility/Utility.h"
#include "../Moment/moment.h"

#include <map>
#include <vector>
#include <algorithm>

uint32_t NEWCET_NODE_ID = 0;
std::map<uint32_t, CETNode*> CLOSED_ITEMSETS;
std::map<uint32_t, std::vector<std::vector<bool>>> TRX_INDEX;

/*struct NewCETNode : virtual public CETNode {
public:
};*/

const uint8_t NOT_CLOSED_NODE = 0x00;

void Append(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, CETNode* const _node, 
  const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right,
  std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE, const uint32_t _BLOCK_SIZE);

void Delete(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, CETNode* const _node, 
  const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right,
  std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE, const uint32_t _BLOCK_SIZE);

//Closure checking
std::vector<uint32_t>* left_check_full(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _TABLE);

/*
void add_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void delete_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void prune_children(CETNode* const _node, const uint32_t _tid, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void remove_from_class(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void update_cetnode_in_hashmap(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void print_cet_node(CETNode* const _node);
*/

//Support computation
void intersect_bitsets(std::vector<bool[]>* const _in1, std::vector<bool[]>* const _in2, const uint32_t _BLOCK_SIZE);
std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _BLOCK_SIZE);
std::vector<std::vector<bool>>* cloneList(std::vector<std::vector<bool>>* const _list);
std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _item, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right);
std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE);
uint32_t support(CETNode* const _node, const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE);
uint32_t support(std::vector<uint32_t>* const _itemset, std::vector<std::vector<bool>>* const _bitset, const uint32_t _window_size, const uint32_t _trx_shift_left,
  const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE);

//CI management
void update_ci_hashmap(CETNode* const _node, const uint32_t _tid, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE);
void register_ci_hashmap(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _TABLE);
void update_index_sliding_window(std::vector<uint32_t>* const _transaction, const uint32_t _i, const uint32_t _trx_shift, const uint32_t _window_size, 
  const uint32_t _MAX_ATTRIBUTES, const uint32_t _BLOCK_SIZE);

//error codes
/*
const uint32_t ERROR_INCORRECT_CREATION_OF_NODE = 0x01;//An invalid node has been created: its support is higher than at least one of its parents (i.e. violates downward closure property)
const uint32_t ERROR_COULD_NOT_FIND_CI_OLD_CLASS = 0x02;//An existing CI's class could not be found in hashtable: either its hash is invalid or the hashtable is not correct (i.e. integrity error)
const uint32_t ERROR_COULD_NOT_FIND_CI = 0x04;//An existing CI class could not be found in hashtable: either its hash is invalid or the hashtable is not correct (i.e. integrity error)
const uint32_t ERROR_DELETE_NODE_WITH_EMPTY_TIDSET = 0x08;//A node has an empty tidset: then it should not exist (i.e. integrity error)
const uint32_t ERROR_DELETE_NODE_WITH_HASH = 0x10;
const uint32_t ERROR_DELETE_NODE_CONTAINS_TRX_BUT_NOT_IN_TIDSET = 0x20;
const uint32_t ERROR_DELETE_REMOVING_INFREQUENT_CI = 0x40;//Removing some infrequent closed node
const uint32_t ERROR_NBR_CLOSED_NODES_DOES_NOT_MATCH_CI_SET_SIZE = 0x80;
const uint32_t ERROR_ID_NEW_CI_ALREADY_REGISTERED = 0x100;
const uint32_t ERROR_CANNOT_DELETE_UNREGISTRED_CI = 0x200;
*/
#endif