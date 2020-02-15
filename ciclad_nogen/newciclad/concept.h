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

#ifndef CONCEPT_HPP
#define	CONCEPT_HPP

#include <cstdint>
#include <vector>
#include <queue>
#include <set>
#include <map>

#define REUSE_OBSOLETE

struct trie_node_add;

#ifdef REUSE_OBSOLETE
  extern std::queue<size_t>* available_id_for_new_cis;
  extern std::queue<size_t>** available_positions_for_new_cis;
#endif

typedef struct concept {
  uint32_t id;
  uint32_t support;
  uint32_t size;
  size_t lastitem;
  size_t* positionsInIndex;
} CONCEPT;

void print_concept(CONCEPT);

#endif	/* CONCEPT_HPP */
