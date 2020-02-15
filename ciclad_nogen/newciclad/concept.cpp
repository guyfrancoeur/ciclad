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

#include <iostream>
#include "concept.h"
/*
void print_concept(CONCEPT concept) {
  uint32_t c = (*concept.itemset);
  std::cout << concept.id << ", concept size is " << c << " with supp " << concept.support << std::endl;
  while (c != 0) {
    std::cout << ", " << concept.itemset[(*concept.itemset + 1) - c];
    --c;
  }
  std::cout << std::endl;
}
*/
#ifdef REUSE_OBSOLETE
  std::queue<size_t>* available_id_for_new_cis = new std::queue<size_t>();
  std::queue<size_t>** available_positions_for_new_cis = 0;
#endif