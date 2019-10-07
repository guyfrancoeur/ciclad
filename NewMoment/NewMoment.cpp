#include "NewMoment.h"


void Append(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, CETNode* const _node,
  const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right,
  std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE, const uint32_t _BLOCK_SIZE) {
  //this computes support of a node, since we do not update tidset or suppport
  //of every node, we have to recompute every time
  uint32_t node_s = _window_size;
  //This is not in the paper but without it, it will not perform...
  if (!contains(_transaction, _node->itemset, false)) {
    return;
  }

  if (_node->itemset->size() == 0 || (node_s = support(_node, _window_size, _trx_shift_left, _trx_shift_right, false)) > _minsupp) {
    _node->support = node_s;
    _node->oldHash = _node->hash;
    _node->hash = _node->support;
    std::vector<uint32_t>* closure;
    std::vector<uint32_t>* missing_items;
    //either the node is <_lex than its closure or if not, some items in the closure are not within the transaction (a new class will be created)
    if (!(closure = left_check(_node, _EQ_TABLE)) || (missing_items = inter(closure, _transaction)).length != closure->size()) {
      //loop sur tous les freres a droite frequents de _node
      //les freres sont tous supposes frequents, sinon on le garde pas
      //si ca fonctionne, il faudrait compter le nombre de generations/tests entre Moment et NewMoment
      if (_node->parent) {
        std::map<uint32_t, CETNode*>* siblings = _node->parent->children;
        std::map<uint32_t, CETNode*>::iterator it = siblings->begin();
        for(; it != siblings->end(); ++it){
          CETNode* const sibling = it->second;
          if (!contains(_transaction, sibling->itemset, false)) {
            continue;
          }
          if (sibling->item > _node->item && (!_node->children || _node->children->end() == _node->children->find(sibling->item))) {
            int sibsup;
            if ((sibsup = support(sibling, _window_size, _trx_shift_left, _trx_shift_right, false)) > _minsupp) {
              std::vector<std::vector<bool>>* childBitset;
              uint32_t supp = support(_node->itemset, childBitset = compute_bitset(_node, sibling->item, _trx_shift_left, _trx_shift_right, _BLOCK_SIZE),
                _window_size, _trx_shift_left, _trx_shift_right, false);
              if (supp > _minsupp) {
                CETNode* const newnode = new CETNode();
                {
                  newnode->support = supp;
                  newnode->id = ++NEWCET_NODE_ID;
                  newnode->itemset = new std::vector<uint32_t>(_node->itemset->begin(), _node->itemset->end());// _node.itemset.length + 1];
                  newnode->itemset->push_back(sibling->itemset->at(sibling->itemset->size() - 1));
                  if (newnode->support > node_s || newnode->support > sibsup) {
                    //System.out.println("incorrect creation of node " + newnode.support + " vs " + node_s + " and " + sibsup);
                    System.exit(1);
                  }
                  newnode->item = sibling->item;
                  newnode->hash = newnode->support;
                  newnode->oldHash = newnode->hash;
                }
                if (!_node->children) {
                  _node->children = new std::map<uint32_t, CETNode*>();
                }
                //System.out.println("Adding node "+Arrays.toString(newnode.itemset)+" to node "+Arrays.toString(_node.itemset));
                _node->children->emplace(sibling->item, newnode);
                newnode->parent = _node;
                NBR_NODES += 1;
              }
            }
          }
        }
      }

      std::map<uint32_t, CETNode*>* const children = _node->children;
      if (children) {
        std::map<uint32_t, CETNode*>::iterator it = children->begin();
        //pour chaque fils de _node, Append(_child)
        for (; it != children->end(); ++it) {
          CETNode* const child = it->second;
          if (!contains(_transaction, child->itemset, false)) {
            continue;
          }
          //should we add > item ?
          Append(_tid, _transaction, _minsupp, child, _window_size, _trx_shift_left, _trx_shift_right, _EQ_TABLE);
        }
      }
      //could be done in the same loop
      bool has_same_support = false;
      if (children) {
        std::map<uint32_t, CETNode*>::iterator it = children->begin();
        //pour chaque fils de _node, Append(_child)
        for (; it != children->end(); ++it) {
          CETNode* const child = it->second;
          uint32_t s;
          if ((s = support(child, _window_size, _trx_shift_left, _trx_shift_right, false)) == /*_node.support*/ node_s) {
            has_same_support = true;
            break;
          }
        }
      }
      if (has_same_support) {
        if (_node->type == CLOSED_NODE && _node->id != 0) {
          //supprimer le closed ? ceci ne devrait pas arriver !!!
          delete_ci(_node, _EQ_TABLE);
          System.exit(1);
        }
      }
      else {
        if (_node->type == CLOSED_NODE) {
          update_ci_hashmap(_node, _tid, _EQ_TABLE);//on le garde
        }
        else {
          //System.out.println("nouveau CI "+Arrays.toString(_node.itemset)+" "+_node.support);
          //nouveau closed (ceci peut arriver en cas de "rupture" de classe d'eq
          _node->type = CLOSED_NODE;
          add_ci(_node, _EQ_TABLE);
          register_ci_hashmap(_node, _EQ_TABLE);
        }
        //this should not happen ?
        //update hashtable ? (could be done in "support")
      }
    }
  }
};

void Delete(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, CETNode* const _node,
  const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right,
  std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE, const uint32_t _BLOCK_SIZE) {
  //System.out.println("AAA dealing with node "+Arrays.toString(_node.itemset));
  if (!contains(_transaction, _node->itemset, false)) {
    return;
  }

  uint32_t node_s = _window_size;
  //calcule si frequent dans D-
  if (_node->itemset->size() == 0 || ((node_s = support(_node, _window_size, _trx_shift_left, _trx_shift_right, false))) > _minsupp) {
    //si noeud est CLOSED
    _node->support = node_s;
    _node->oldHash = _node->hash;
    _node->hash = _node->support;

    if (_node->parent) {
      std::map<uint32_t, CETNode*>* siblings = _node->parent->children;
      std::map<uint32_t, CETNode*>::iterator it = siblings->begin();
      for (; it != siblings->end(); ++it) {
        CETNode* const sibling = it->second;
        if (!contains(_transaction, sibling->itemset, false)) {
          continue;
        }
      }
    }
    //pour chaque fils, call Remove
    std::map<uint32_t, CETNode*>* const children = _node->children;
    if (children) {
      std::map<uint32_t, CETNode*>::iterator it = children->begin();
      for (; it != children->end(); ++it) {
        CETNode* child = it->second;
        //si on le teste pas ici, ca devrait marcher aussi
        if (!contains(_transaction, child->itemset, false)) {
          continue;
        }
        //should we add > item ?
        Delete(_tid, _transaction, _minsupp, child, _window_size, _trx_shift_left, _trx_shift_right, _EQ_TABLE);
      }
    }

    //si node est frequent dans D
    //a quoi sert ceci ?
    //System.out.println("dealing with node "+Arrays.toString(_node.itemset));
    if (node_s > _minsupp) {

      std::vector<uint32_t>* closure;
      //System.out.println("supp="+node_s+" and hash is "+_node.hash);
      if (!(closure = left_check_full(_node, _EQ_TABLE))) {
        if (_node->type == CLOSED_NODE) {
          //update table
          update_ci_hashmap(_node, _tid, _EQ_TABLE);
        }
        else {
          if (_node->id != 0) {
            //ceci ne devrait jamais arriver !!!!
            exit(1);
            //nouveau closed frequent
            add_ci(_node, _EQ_TABLE);
            register_ci_hashmap(_node, _EQ_TABLE);
          }
        }
      }
      else {
        if (_node->type == CLOSED_NODE) {
          //delete ci
          delete_ci(_node, _EQ_TABLE);
          _node->type = 0x00;
          //_node.parent.children.remove(_node.item);
        }
      }
    }
    else {
      if (_node->type == CLOSED_NODE) {
        //non closed et elimine
        delete_ci(_node, _EQ_TABLE);
        _node->type = NOT_CLOSED_NODE;
        //_node.parent.children.remove(_node.item);
      }
    }
  }
  else {
    _node->support = node_s;
    _node->oldHash = _node->hash;
    _node->hash = _node->support;
    //System.out.println("lol dealing with node "+Arrays.toString(_node.itemset)+" because support ");
    prune_children(_node, _tid, _EQ_TABLE);
    if (_node->type == CLOSED_NODE) {
      //non closed et elimine
      delete_ci(_node, _EQ_TABLE);
      _node->type = NOT_CLOSED_NODE;
      //_node.parent.children.remove(_node.item);
    }
  }
};


std::vector<uint32_t>* left_check_full(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (_EQ_TABLE->end() != _EQ_TABLE->find(_node->hash)) {
    std::vector<std::vector<CETNode*>*>* const potential_closures_by_size = _EQ_TABLE->find(_node->hash)->second;
    for (int i = _node->itemset->size() + 1; i < potential_closures_by_size->size(); ++i) {
      std::vector<CETNode*>* const strate = potential_closures_by_size->at(i);
      std::vector<CETNode*>::iterator it = strate->begin();
      for (; it != strate->end(); ++it) {
        CETNode* const candidate = *it;
        if (candidate->support == _node->support && contains(candidate->itemset, _node->itemset, true)) {
          std::vector<uint32_t>* const set_difference = diff(candidate->itemset, _node->itemset);
          return set_difference;
        }
      }
    }
  }
  return 0;
};

void intersect_bitsets(std::vector<std::vector<bool>>* const _in1, std::vector<std::vector<bool>>* const _in2, const uint32_t _BLOCK_SIZE) {
  if (_in1->size() != _in2->size()) {
    //System.out.println("bitsets are not same size!!!! " + _in1->size() + " vs " + _in2->size());
    System.exit(1);
  }
  for (uint32_t j = 0; j != _in1->size(); ++j) {
    std::vector<bool>* const bitset_left = &_in1->at(j);
    std::vector<bool>* const bitset_right = &_in2->at(j);
    for (int z = 0; z != _BLOCK_SIZE; ++z) {
      bitset_left->at(z) = bitset_left->at(z) & bitset_right->at(z);
    }
  }
  return;
};

std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _BLOCK_SIZE) {
  std::vector<uint32_t>* const itemset = _node->itemset;
  if (TRX_INDEX.end() == TRX_INDEX.find(itemset->at(0))) {
    //System.out.println("error : item missing in index..."+itemset[0]+" "+Arrays.toString(itemset));
    //System.exit(1);
    return new std::vector<std::vector<bool>>();//PAS BON!!!!!
  }
  std::vector<std::vector<bool>>* const current = cloneList(&TRX_INDEX.find(itemset->at(0))->second);//.clone();
  //on calcule intersection de tous les tids
  for (uint32_t i = 1; i != itemset->size(); ++i) {
    if (TRX_INDEX.end() == TRX_INDEX.find(itemset->at(i))) {
      return new std::vector<std::vector<bool>>();//PAS BON!!!!!
    }
    std::vector<std::vector<bool>>* const occurences = &TRX_INDEX.find(itemset->at(i))->second;
    intersect_bitsets(current, occurences, _BLOCK_SIZE);
  }
  return current;
};

std::vector<std::vector<bool>>* cloneList(std::vector<std::vector<bool>>* const _list) {
  std::vector<std::vector<bool>>* const clone = new std::vector<std::vector<bool>>();//copier la structure
  std::vector<std::vector<bool>>::iterator it = clone->begin();
  for (; it != clone->end(); ++it) {
    std::vector<bool>* const item = &(*it);
    clone->push_back(std::vector<bool>(item->begin(), item->end()));
  }
  return clone;
};
std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _item, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE) {
  std::vector<std::vector<bool>>* const bitset_parent_left = compute_bitset(_node, _BLOCK_SIZE);
  std::vector<std::vector<bool>>* const item_bitset = cloneList(&TRX_INDEX.find(_item)->second);//.clone();
  intersect_bitsets(bitset_parent_left, item_bitset, _BLOCK_SIZE);
  return bitset_parent_left;
};

//This should be removed
std::vector<std::vector<bool>>* compute_bitset(CETNode* const _node, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE) {
  std::vector<std::vector<bool>>* const bitset_parent_left = compute_bitset(_node, _BLOCK_SIZE);
  return bitset_parent_left;
};

uint32_t support(CETNode* const _node, const uint32_t _window_size, const uint32_t _trx_shift_left, const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE) {
  return support(_node->itemset, compute_bitset(_node, _BLOCK_SIZE), _window_size, _trx_shift_left, _trx_shift_right, _BLOCK_SIZE);
};

uint32_t support(std::vector<uint32_t>* const _itemset, std::vector<std::vector<bool>>* const _bitset, const uint32_t _window_size, const uint32_t _trx_shift_left, 
  const uint32_t _trx_shift_right, const uint32_t _BLOCK_SIZE) {
  uint32_t support = 0;
  //System.out.println("computing support for "+Arrays.toString(_itemset)+" shift="+_trx_shift);
  if (_bitset->empty()) {
    return 0;
  }

  uint32_t const space_in_last_block = _window_size % _BLOCK_SIZE;
  if ((space_in_last_block + _trx_shift_left) > _BLOCK_SIZE) {
    //on est sur tous les blocks
    //le dernier block on doit 
    uint32_t const additional_space = (space_in_last_block + _trx_shift_right) % _BLOCK_SIZE;
    const uint32_t max_block = _bitset->size() - 1;
    //only count positions at 1 on non-extreme blocs
    for (uint32_t j = 1; j < max_block; ++j) {
      std::vector<bool>* const bitset_left = &_bitset->at(j);
      for (uint32_t z = 0; z != _BLOCK_SIZE; ++z) {
        if (bitset_left->at(z)) {
          support += 1;
        }
      }
    }

    {
      std::vector<bool>* const bitset_last = &_bitset->at(max_block);
      for (uint32_t z = 0; z < additional_space; ++z) {
        if (bitset_last->at(z)) {
          support += 1;
        }
      }
    }

    //for extreme blocks only count until "shift"
    //A VERIFIER CECI
    {
      std::vector<bool>* bitset_first = &_bitset->at(0);
      for (uint32_t z = _trx_shift_left; z != _BLOCK_SIZE; ++z) {
        if (bitset_first->at(z)) {
          support += 1;
        }
      }
    }
  }
  else {
    //on est sur tous sauf un
    const uint32_t additional_space = (space_in_last_block + _trx_shift_right) % _BLOCK_SIZE;
    if (_window_size < _BLOCK_SIZE) {
      {
        std::vector<bool>* const bitset_first = &_bitset->at(1);
        for (uint32_t z = _trx_shift_left; z != (_trx_shift_right + _window_size); ++z) {
          if (bitset_first->at(z)) {
            support += 1;
          }
        }
      }
    }
    else {
      {
        std::vector<bool>* bitset_first = &_bitset->at(1);
        for (int z = _trx_shift_left; z != _BLOCK_SIZE; ++z) {
          if (bitset_first->at(z)) {
            support += 1;
          }
        }
      }
      const uint32_t max_block = _bitset->size() - 1;
      //only count positions at 1 on non-extreme blocs
      for (int j = 2; j < max_block; ++j) {
        std::vector<bool>* const bitset_left = &_bitset->at(j);
        for (int z = 0; z != _BLOCK_SIZE; ++z) {
          if (bitset_left->at(z)) {
            support += 1;
          }
        }
      }

      {
        std::vector<bool>* const bitset_last = &_bitset->at(max_block);
        for (int z = 0; z < additional_space; ++z) {
          if (bitset_last->at(z)) {
            support += 1;
          }
        }
      }
    }
  }
  return support;
};

void update_ci_hashmap(CETNode* const _node, const uint32_t _tid, std::map<long, std::vector<std::vector<CETNode*>>>* const _EQ_TABLE) {
  //mettre a jour la table de hashage : changer de classe d'equivalence
  const long oldTidSum = _node->oldHash;//ni_prime.tidsum - _tid;
  if (_EQ_TABLE->end() == _EQ_TABLE->find(oldTidSum)) {
    //erreur ?
    //System.out.println("could not find CI's old class...");
    System.exit(1);
  }
  else {
    // remove in old class
    {
      std::vector<CETNode*>* const ancienneClasse = &(&_EQ_TABLE->find(oldTidSum)->second)->at(_node->itemset->size());
      uint32_t positionMatch = ancienneClasse->size();
      uint32_t cursor = 0;

      std::vector<CETNode*>::iterator it = ancienneClasse->begin();
      for (; it != ancienneClasse->end(); ++it) {
        CETNode* const n = *it;
        if (exactMatch(n->itemset, _node->itemset)) {
          positionMatch = cursor;//ok trouve, on doit le supprimer
          break;
        }
        cursor += 1;
      }
      if (positionMatch != ancienneClasse->size()) {
        ancienneClasse->erase(ancienneClasse->begin() + positionMatch);
      }
      else {
        //erreur
        System.out.println("could not find CI to transfer between classes...");
        System.exit(1);
      }
    }

    //add into new class
    //on ajoute le noeud dans la bonne classe d'equivalence (tidsum + support)
    {
      if (_EQ_TABLE->end() == _EQ_TABLE->find(_node->hash)) {
        _EQ_TABLE->emplace(_node->hash, new std::vector<std::vector<CETNode*>*>());
      }
      if ((&_EQ_TABLE->find(_node->hash)->second)->size() <= _node->itemset->size()) {
        while ((&_EQ_TABLE->find(_node->hash)->second)->size() <= _node->itemset->size()) {
          (&_EQ_TABLE->find(_node->hash)->second)->push_back(std::vector<CETNode*>());
        }
      }
      (&(&_EQ_TABLE->find(_node->hash)->second)->at(_node->itemset->size()))->push_back(_node);
    }
  }
  return;
};

void register_ci_hashmap(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>>>* const _TABLE) {
  if (_TABLE->end() == _TABLE->find(_node->hash)) {
    _TABLE->emplace(_node->hash, std::vector<std::vector<CETNode*>>());
  }
  std::vector<std::vector<CETNode*>>* const all_strates = &_TABLE->find(_node->hash)->second;
  while (all_strates->size() <= _node->itemset->size()) {
    all_strates->push_back(std::vector<CETNode*>());
  }
  all_strates->at(_node->itemset->size()).push_back(_node);
};

void update_index_sliding_window(std::vector<uint32_t>* const _transaction, const uint32_t _i, const uint32_t _trx_shift, 
  const uint32_t _window_size, const uint32_t _MAX_ATTRIBUTES, const uint32_t _BLOCK_SIZE) {
  {
    if (_i < _window_size) {
      for (uint32_t j = 0; j != _transaction->size(); ++j) {
        if (TRX_INDEX.find(_transaction->at(j)) == TRX_INDEX.end()) {
          TRX_INDEX.emplace(_transaction->at(j), std::vector<bool[]>());
        }
        //il faut ajouter autant de blocs qu'on a de window_size / blocksize
        if ((&TRX_INDEX.find(_transaction->at(j))->second)->empty()) {
          const uint32_t nbr_blocks = 1 + ((uint32_t)std::ceil(_window_size / (float)_BLOCK_SIZE));
          for (uint32_t z = 0; z != nbr_blocks; ++z) {
            std::vector<bool>* const next_block = &std::vector<bool>();
            next_block->resize(_BLOCK_SIZE);
            (&TRX_INDEX.find(_transaction->at(j))->second)->push_back(*next_block);//full copy
          }
        }
        std::vector<bool>* const current_block = &(&TRX_INDEX.find(_transaction->at(j))->second)->at(1 + (uint32_t)std::floor(_i / (float)_BLOCK_SIZE));
        current_block->at(_i % _BLOCK_SIZE) = true;
      }
    } //dans le cas sliding window (forget and add)
    else {
      if (_trx_shift == 0) {
        //ici loop sur tous les items
        for (int j = 0; j != _MAX_ATTRIBUTES; ++j) {
          //si l'item fait partie de la transaction
          if (binary_search(_transaction->begin(), _transaction->end(), j)) {
            if (TRX_INDEX.find(j) == TRX_INDEX.end()) {
              TRX_INDEX.emplace(j, std::vector<bool[]>());
            }
            //si on doit creer un nouveeau block ?
            if ((&TRX_INDEX.find(j)->second)->empty()) {
              std::vector<bool>* const new_block = &std::vector<bool>();
              new_block->resize(_BLOCK_SIZE);
              new_block->at(_i % _BLOCK_SIZE) = true;
              //il faut ajouter autant de blocs qu'on a de window_size / blocksize
              const uint32_t nbr_blocks = ((uint32_t)std::ceil(_window_size / (float)_BLOCK_SIZE));
              for (uint32_t z = 0; z != nbr_blocks; ++z) {
                std::vector<bool>* const next_block = &std::vector<bool>();
                next_block->resize(_BLOCK_SIZE);
                (&TRX_INDEX.find(j)->second)->push_back(*next_block);
              }
              (&TRX_INDEX.find(j)->second)->push_back(*new_block);
              //System.out.println(j + " - created " + nbr_blocks + " blocks + 1");
              //System.out.println("" + TRX_INDEX.get(j).toString());
            }
            else {
              //on oublie le dernier block, et on le reutilise pour le nouveau block !!!
              std::vector<bool>* const first_block = &(&TRX_INDEX.find(j)->second)->at(0);
              //System.out.println("oldest block has " + Arrays.toString(first_block));
              //Arrays.fill(first_block, false);
              std::fill(first_block->begin(), first_block->end(), false);
              first_block->at(_i % _BLOCK_SIZE) = true;
              (&TRX_INDEX.find(j)->second)->erase((&TRX_INDEX.find(j)->second)->begin());//TODO: rewrite this
              (&TRX_INDEX.find(j)->second)->push_back(*first_block);
              //System.out.println(j + " - replaced oldest block at " + (TRX_INDEX.get(j).size() - 1));
              //System.out.println("" + TRX_INDEX.get(j).toString());
            }
          } //si il ne fait pas partie
          else {
            if (TRX_INDEX.find(j) != TRX_INDEX.end() && !(&TRX_INDEX.find(j)->second)->empty()) {
              //prendre le dernier block, le reinitialiser tout a zero
              std::vector<bool>* const first_block = &(&TRX_INDEX.find(j)->second)->at(0);
              //System.out.println("oldest block has " + Arrays.toString(first_block));
              //Arrays.fill(first_block, false);
              std::fill(first_block->begin(), first_block->end(), false);
              (&TRX_INDEX.find(j)->second)->erase((&TRX_INDEX.find(j)->second)->begin());//TODO: rewrite this
              (&TRX_INDEX.find(j)->second)->push_back(*first_block);
              //System.out.println(j + " - replaced oldest block at " + (TRX_INDEX.get(j).size() - 1));
              //System.out.println("" + TRX_INDEX.get(j).toString());
            }
          }
        }
      }
      else {
        for (uint32_t j = 0; j != _transaction->size(); ++j) {
          //si c'est la premiere fois qu'on rencontre cet item
          if (TRX_INDEX.find(_transaction->at(j)) == TRX_INDEX.end()) {
            TRX_INDEX.emplace(_transaction->at(j), std::vector<std::vector<bool>>());
          }
          //me chose en dessous
          if ((&TRX_INDEX.find(_transaction->at(j))->second)->empty()) {
            std::vector<bool>* const new_block = &std::vector<bool>();
            new_block->resize(_BLOCK_SIZE);// ];
            new_block->at(_i % _BLOCK_SIZE) = true;
            const uint32_t nbr_blocks = ((uint32_t)std::ceil(_window_size / (float)_BLOCK_SIZE));
            for (uint32_t z = 0; z != nbr_blocks; ++z) {
              std::vector<bool>* const next_block = &std::vector<bool>();// BLOCK_SIZE];
              next_block->resize(_BLOCK_SIZE);
              (&TRX_INDEX.find(_transaction->at(j))->second)->push_back(*next_block);
            }
            (&TRX_INDEX.find(_transaction->at(j))->second)->push_back(*new_block);
          }
          else {
            //ceci est le cas normal, on prend le bloc courant et on rajouter un bit
            const uint32_t debut_pos2 = _BLOCK_SIZE * ((&TRX_INDEX.find(_transaction->at(j))->second)->size() - 1);
            const uint32_t fin_pos2 = (_BLOCK_SIZE * (1 + (&TRX_INDEX.find(_transaction->at(j))->second)->size() - 1)) - 1;
            //System.out.println("on prend le dernier block " + debut_pos2 + " -> " + fin_pos2);
            std::vector<bool>* const last_block = &(&TRX_INDEX.find(_transaction->at(j))->second)->at((&TRX_INDEX.find(_transaction->at(j))->second)->size() - 1);
            last_block->at(_i % _BLOCK_SIZE) = true;
          }
        }
      }
    }
    //System.out.println("index for item="+new_transaction[j]+" " +Arrays.deepToString(TRX_INDEX.get(new_transaction[j]).toArray(new boolean[0][0])));
  }
};
