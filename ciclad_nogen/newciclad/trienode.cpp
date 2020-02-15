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

#include "trienode.h"

std::vector<uint32_t>* trie_path(std::vector<trie_node_add>* tnodes, trie_node_add* const node) {
  trie_node_add nodeRef = *node;
  const uint32_t pathSize = nodeRef.depth;
  std::vector<uint32_t>* path = new std::vector<uint32_t>();
  path->resize(pathSize);
  for (uint16_t d = pathSize - 1; d != 0; --d) {
    path->at(d) = nodeRef.item;
    nodeRef = (*tnodes)[nodeRef.parent];
  }
  path->at(0) = nodeRef.item;

  return path;
}

std::vector<uint32_t>* trie_path(std::vector<trie_node_rmv>* tnodes, trie_node_rmv* const node) {
  trie_node_rmv nodeRef = *node;
  const uint32_t pathSize = nodeRef.depth;
  std::vector<uint32_t>* path = new std::vector<uint32_t>();
  path->resize(pathSize);
  for (uint16_t d = pathSize - 1; d != 0; --d) {
    path->at(d) = nodeRef.item;
    nodeRef = (*tnodes)[nodeRef.parent];
  }
  path->at(0) = nodeRef.item;
  return path;
}


