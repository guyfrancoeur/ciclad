#ifndef _CICLAD_REMOVE_H
#define _CICLAD_REMOVE_H

#include <cstdio>
#include <vector>
#include <queue>

#include "isci.h"

using namespace std;

void calculateRemove(TRANSACTION *current_T, vector<vector<concept*>>*index, vector<concept*>*conceptContainer);
void expandPath(uint32_t item, size_t ref_pos, concept* ci, vector<trie_node_rmv>* tnodes);

#endif