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

#ifndef CicladRmv_hpp
#define CicladRmv_hpp

using namespace std;
#include <stdio.h>
#include <vector>
#include <queue>
#include "transaction.h"
#include "concept.h"
#include "trienode.h"

void updateCicladRmv(TRANSACTION *current_T,vector<vector<concept*>>*index, vector<concept*>*conceptContainer);
void expandPathRmv(uint32_t item, size_t ref_pos, concept* ci, vector<trie_node_rmv>* tnodes);
#endif /* CicladRmv_hpp */
