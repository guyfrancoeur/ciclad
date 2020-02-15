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

#ifndef FASTTRIE_HPP
#define	FASTTRIE_HPP

#include <map>
#include <vector>
struct concept;

typedef struct trie_node_add {
  uint32_t nb_ref;
  concept* genitor;
  uint32_t depth;
  uint32_t item;
  size_t parent;
  std::map<uint32_t, size_t>* children;
} TRIE_NODE_ADD;

typedef struct trie_node_rmv{
    uint32_t nb_ref;
    std::vector<concept*>* genitors;
    std::vector<concept*>* minimalCandidates;
    uint32_t depth;
    uint32_t item;
    size_t parent;
    std::map<uint32_t, size_t>* children;
}TRIE_NODE_RMV;

std::vector<uint32_t>* trie_path(std::vector<trie_node_add>* tnodes, trie_node_add* const node);
std::vector<uint32_t>* trie_path(std::vector<trie_node_rmv>* tnodes, trie_node_rmv* const node);
#endif
