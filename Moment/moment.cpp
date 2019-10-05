#include "moment.h"
#include "../CommonUtility/Utility.h"
#include <iostream>

uint32_t CET_NODE_ID = 0;
uint32_t NBR_NODES = 0;
uint32_t NBR_CLOSED_NODES = 0;

std::map<uint32_t, CETNode*> CLOSED_ITEMSETS;

void Explore(const uint32_t _tid, CETNode* const _node, std::vector<uint32_t>* const _transaction, const uint32_t _minsupp, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (_node->support < _minsupp) {
    if (_node->type == CLOSED_NODE) {
      delete_ci(_node, _EQ_TABLE);
    }
    _node->type = INFREQUENT_GATEWAY_NODE;
    prune_children(_node, _tid, _EQ_TABLE);
  }
  else if (left_check(_node, _EQ_TABLE)) {
    if (_node->type == CLOSED_NODE) {
      delete_ci(_node, _EQ_TABLE);
    }
    _node->type = UNPROMISSING_GATEWAY_NODE;
    prune_children(_node, _tid, _EQ_TABLE);
  }
  else {
    //combiner les freres a droite
    std::map<uint32_t, CETNode*>::iterator it = _node->parent->children->begin();
    for (; it != _node->children->end(); ++it) {
      if (it->first > _node->item && binary_search(_transaction->begin(), _transaction->end(), it->first)) {
        CETNode* const sibling = it->second;
        if (sibling->support > _minsupp && (!_node->children || _node->children->end() == _node->children->find(it->first))) {
          CETNode* const newnode = new CETNode();
          //compute support of a new node (support, tidset, tidsum)
          {
            std::vector<uint32_t>* const left_tids = _node->tidlist;
            std::vector<uint32_t>* const right_tids = sibling->tidlist;
            newnode->id = ++CET_NODE_ID;
            newnode->tidlist = inter(left_tids, right_tids);
            newnode->support = newnode->tidlist->size();
            newnode->itemset = new std::vector<uint32_t>(_node->itemset->begin(), _node->itemset->end());// [_node->itemset->size() + 1];
            newnode->item = it->first;
            //System.arraycopy(_node->itemset->size(), newnode->itemset, 0, _node->itemset.length);
            newnode->itemset->push_back(sibling->itemset->at(sibling->itemset->size() - 1));
            for (int z = 0; z != newnode->tidlist->size(); ++z) {
              newnode->tidsum += newnode->tidlist->at(z);
            }
            newnode->hash = newnode->tidsum * newnode->support;
            newnode->oldHash = newnode->hash;
          }
          if (!_node->children) {
            _node->children = new std::map<uint32_t, CETNode*>();
          }
          _node->children->emplace(it->first, newnode);
          newnode->parent = _node;
          NBR_NODES += 1;
        }
      }
    }

    bool has_same_support = false;
    if (_node->children) {
      std::map<uint32_t, CETNode*>::iterator it = _node->children->begin();
      for (; it != _node->children->end(); ++it) {
        CETNode* const next = it->second;
        Explore(_tid, next, _transaction, _minsupp, _EQ_TABLE);
      }
    }
    if (_node->children) {
      std::map<uint32_t, CETNode*>::iterator it = _node->children->begin();
      for (; it != _node->children->end(); ++it) {
        CETNode* const next = it->second;
        if (next->support == _node->support) {
          has_same_support = true;
          break;
        }
      }
    }
    //si il existe un fils de _node tel que egal support alors _node devient intermediate
    if (has_same_support) {
      if (_node->type == CLOSED_NODE) {
        delete_ci(_node, _EQ_TABLE);
      }
      _node->type = INTERMEDIATE_NODE;
    }
    else {
      if (_node->type != CLOSED_NODE) {
        _node->type = CLOSED_NODE;
        add_ci(_node, _EQ_TABLE);
        //This should go into a function
        {
          if (_EQ_TABLE->end() == _EQ_TABLE->find(_node->hash)) {
            _EQ_TABLE->emplace(_node->hash, new std::vector<std::vector<CETNode*>*>());
          }
          if (_EQ_TABLE->find(_node->hash)->second->size() <= _node->itemset->size()) {
            while (_EQ_TABLE->find(_node->hash)->second->size() <= _node->itemset->size()) {
              _EQ_TABLE->find(_node->hash)->second->push_back(new std::vector<CETNode*>());
            }
          }
          _EQ_TABLE->find(_node->hash)->second->at(_node->itemset->size())->push_back(_node);
        }
      }
    }
  }
};

void Addition(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, const CETNode* _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  //not relevant to addition => not included in transaction
  if (!contains(_transaction, _node->itemset, false)) {
    return;
  }

  std::vector<uint32_t> newly_promissing_nodes;
  std::vector<uint32_t> newly_frequent_nodes;
  std::vector<uint32_t> frequent_nodes;
  if (!_node->children) {
    return;
  }

  std::map<uint32_t, CETNode*>::iterator it = _node->children->begin();
  for (; it != _node->children->end(); ++it) {
    CETNode* const childNode = it->second;
    if (contains(_transaction, childNode->itemset, false)) {
      //increment support of already existing node (support, tidset, tidsum)
      {
        if (childNode->tidlist->size() == 0 || childNode->tidlist->at(childNode->tidlist->size() - 1) != _tid) {
          childNode->support += 1;
          childNode->tidsum += _tid;

          childNode->oldHash = childNode->hash;
          childNode->hash = childNode->tidsum * childNode->support;

          std::vector<uint32_t>* tmpTidList = new std::vector<uint32_t>(childNode->tidlist->begin(), childNode->tidlist->end());
          tmpTidList->push_back(_tid);
          childNode->tidlist = tmpTidList;
        }

      }
      if (childNode->support > _minsupp) {
        frequent_nodes.push_back(it->first);
      }

      if (childNode->type == UNPROMISSING_GATEWAY_NODE) {
        //dans ce cas, meme si non prometteur il est quand meme frequent donc
        //ses combinaisons en tant que right sibling existent deja
        //il faut verifier si il devient prometteur ou pas
        std::vector<uint32_t>* diff;
        if ((diff = left_check(childNode, _EQ_TABLE))) {
          bool missing_element = false;
          for (int j = 0; j != diff->size(); ++j) {
            if (!binary_search(_transaction->begin(), _transaction->end(), diff->at(j))) {
              missing_element = true;
              break;
            }
          }
          if (missing_element) {
            newly_promissing_nodes.push_back(it->first);
          }
        }
      }
      else if (childNode->type == INFREQUENT_GATEWAY_NODE) {
        //ces noeuds la etaient infrequent donc leurs combinaisons en tant que freres a droite n'existent pas encore
        //leurs coimbinaisons en tant que parent canonique non plus
        //donc il faut verifier leur frequence uniquement.
        //a noter qui le passage de infrequent a frequent brise la CLEQ donc meme si il etait non-prmetteur il deiendrait prometteur
        if (childNode->support > _minsupp) {
          newly_frequent_nodes.push_back(it->first);
        }
      }
      else if (childNode->type == INTERMEDIATE_NODE) {
        //ces noeuds sont frequents, prometteurs mais leur fermeture ce trouve direcement plus bas dans l'arbre/
        //ces noeuds sont deja utilises comme parent canoniques (nromalement, seulement un par niveau, donc probablement pas comme frere a droite)
        //rien a faire avec eux ? si, ils peuvent devenir interessants si la nouvelle transaction brise leur classe d'equivalence
        //mais cela va se detecter tout seul via la recursion (existe fils avec support !=)
      }
    }
  }
  
  it = _node->children->begin();
  for (; it != _node->children->end(); ++it) {
    CETNode* const ni_prime = it->second;
    if (!contains(_transaction, ni_prime->itemset, false)) {
      continue;
    }
    std::vector<uint32_t>* closure;
    if (ni_prime->support < _minsupp) {
      if (ni_prime->type == CLOSED_NODE) {
        //was closed but is now infrequent gateway
        delete_ci(ni_prime, _EQ_TABLE);
      }
      ni_prime->type = INFREQUENT_GATEWAY_NODE;
      prune_children(ni_prime, _tid, _EQ_TABLE);
      //becomes infrequent gateway
    }
    //je suppose qu'il faudrai tester si closed ou pas ?
    //parce qui si closed et on trouve le noeud lui meme, alors ca devrait pas fonctionner vu qu'on va le marquer comme unpromissing ?
    else if (ni_prime->type != CLOSED_NODE && (closure = left_check(ni_prime, _EQ_TABLE))) {
      //le cas ou est CLOSED ne devrait JAMAIS arriver !!!!!
      if (ni_prime->type == CLOSED_NODE) {
        delete_ci(ni_prime, _EQ_TABLE);
        //was closed but is now unpromissing gateway
      }
      ni_prime->type = UNPROMISSING_GATEWAY_NODE;
      prune_children(ni_prime, _tid, _EQ_TABLE);
      //becomes unpromissing, positive leftcheck
    }
    else {
      if (binary_search(newly_frequent_nodes.begin(), newly_frequent_nodes.end(), ni_prime->item)
        || binary_search(newly_promissing_nodes.begin(), newly_promissing_nodes.end(), ni_prime->item)) {
        //is among newly promissing or frequent, will start recursive exploration
        Explore(_tid, ni_prime, _transaction, _minsupp, _EQ_TABLE);
      }
      else {
        //pour chaque nouveau frequent tel que I' < K (???) => freres a droite ?
        std::vector<uint32_t>::iterator frequent_siblings = frequent_nodes.begin();
        for (; frequent_siblings != frequent_nodes.end(); ++frequent_siblings) {
          if (*frequent_siblings > ni_prime->item && (!ni_prime->children || ni_prime->children->end() == ni_prime->children->find(*frequent_siblings))) {
            CETNode* newnode = new CETNode();
            CETNode* sibling = _node->children->find(*frequent_siblings)->second;
            //compute support of a new node (support, tidset, tidsum)
            {
              std::vector<uint32_t>* const left_tids = ni_prime->tidlist;
              std::vector<uint32_t>* const right_tids = sibling->tidlist;
              newnode->id = ++CET_NODE_ID;
              newnode->tidlist = inter(left_tids, right_tids);
              newnode->itemset = new std::vector<uint32_t>(ni_prime->itemset->begin(), ni_prime->itemset->end());
              //newnode->itemset->resize(ni_prime->itemset->size()+1);
              //System.arraycopy(ni_prime.itemset, 0, newnode.itemset, 0, ni_prime.itemset.length);
              newnode->itemset->push_back(sibling->itemset->at(sibling->itemset->size() - 1));
              newnode->support = newnode->tidlist->size();
              if (newnode->support > ni_prime->support || newnode->support > sibling->support) {
                exit(ERROR_INCORRECT_CREATION_OF_NODE);
              }
              newnode->item = *frequent_siblings;
              for (uint32_t z = 0; z != newnode->tidlist->size(); ++z) {
                newnode->tidsum += newnode->tidlist->at(z);
              }
              newnode->hash = newnode->tidsum * newnode->support;
              newnode->oldHash = newnode->hash;
            }
            if (!ni_prime->children) {
              ni_prime->children = new std::map<uint32_t, CETNode*>();
            }
            ni_prime->children->emplace(*frequent_siblings, newnode);
            newnode->parent = ni_prime;
            NBR_NODES += 1;
          }
        }
       
        Addition(_tid, _transaction, _minsupp, ni_prime, _EQ_TABLE);
        if (ni_prime->type == CLOSED_NODE) {
          //mettre a jour la table de hashage : changer de classe d'equivalence
          if (ni_prime->tidlist->size() != 0 && ni_prime->tidlist->at(ni_prime->tidlist->size() - 1) == _tid) {
            const long oldTidSum = ni_prime->oldHash;//ni_prime.tidsum - _tid;
            if (_EQ_TABLE->end() == _EQ_TABLE->find(oldTidSum)) {
              exit(ERROR_COULD_NOT_FIND_CI_OLD_CLASS);
            }
            else {
              // remove in old class
              {
                //std::map<long, std::vector<std::vector<CETNode>*>*>* const _EQ_TABLE
                std::vector<CETNode*>* const ancienneClasse = (_EQ_TABLE->find(oldTidSum)->second)->at(ni_prime->itemset->size());
                uint32_t positionMatch = ancienneClasse->size();
                uint32_t cursor = 0;

                std::vector<CETNode*>::iterator strate = ancienneClasse->begin();
                for (; strate != ancienneClasse->end(); ++strate) {
                  CETNode* const n = *strate;
                  if (exactMatch(n->itemset, ni_prime->itemset)) {
                    positionMatch = cursor;//ok trouve, on doit le supprimer
                    break;
                  }
                  cursor += 1;
                }
                if (positionMatch != ancienneClasse->size()) {
                  ancienneClasse->erase(ancienneClasse->begin() + positionMatch);
                }
                else {
                  exit(ERROR_COULD_NOT_FIND_CI);
                }
              }

              //add into new class
              //on ajoute le noeud dans la bonne classe d'equivalence (tidsum + support)
              {
                //This shoudl go into a function
                if (_EQ_TABLE->end() == _EQ_TABLE->find(ni_prime->hash)) {
                  _EQ_TABLE->emplace(ni_prime->hash, new std::vector<std::vector<CETNode*>*>());
                }
                if (_EQ_TABLE->find(ni_prime->hash)->second->size() <= ni_prime->itemset->size()) {
                  while (_EQ_TABLE->find(ni_prime->hash)->second->size() <= ni_prime->itemset->size()) {
                    _EQ_TABLE->find(ni_prime->hash)->second->push_back(new std::vector<CETNode*>());
                  }
                }
                _EQ_TABLE->find(ni_prime->hash)->second->at(ni_prime->itemset->size())->push_back(ni_prime);
              }
            }
          }
          else {
            //ceci ne devrait JAMAIS arriver non plus ? en fait si c'est possible
          }
        }
        else {
          //si il existe pas un fils de ni_prime avec le meme support
          //mettre a jour la table de hashage avec ni_prime
          bool has_child_with_same_support = false;
          if (ni_prime->children) {
            std::map<uint32_t, CETNode*>::iterator children = ni_prime->children->begin();
            for (; children != ni_prime->children->end(); ++children) {
              CETNode* const ni_second = children->second;
              if (ni_second->support == ni_prime->support) {
                has_child_with_same_support = true;
                break;
              }
            }
          }
          if (!has_child_with_same_support) {
            ni_prime->type = CLOSED_NODE;
            add_ci(ni_prime, _EQ_TABLE);
            {
              //This shoudl go into a function
              if (_EQ_TABLE->end() == _EQ_TABLE->find(ni_prime->hash)) {
                _EQ_TABLE->emplace(ni_prime->hash, new std::vector<std::vector<CETNode*>*>());
              }
              if (_EQ_TABLE->find(ni_prime->hash)->second->size() <= ni_prime->itemset->size()) {
                while (_EQ_TABLE->find(ni_prime->hash)->second->size() <= ni_prime->itemset->size()) {
                  _EQ_TABLE->find(ni_prime->hash)->second->push_back(new std::vector<CETNode*>());
                }
              }
              _EQ_TABLE->find(ni_prime->hash)->second->at(ni_prime->itemset->size())->push_back(ni_prime);
            }
          }
          else {
            if (ni_prime->type == CLOSED_NODE) {
              delete_ci(ni_prime, _EQ_TABLE);
            }
            ni_prime->type = INTERMEDIATE_NODE;//est ce bon ici ? pas au dessus ?
            //normalement c'est bon, verifier qu'il etait pas d'un autre type
          }
        }
      }
    }
  }
};

void Deletion(const uint32_t _tid, std::vector<uint32_t>* _transaction, const uint32_t _minsupp, CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (!contains(_transaction, _node->itemset, false)) {
    return;
  }
  std::vector<uint32_t> newly_infrequent_nodes;
  std::vector<uint32_t> infrequent_nodes;
  if (!_node->children) {
    return;
  }

  std::map<uint32_t, CETNode*>::iterator it = _node->children->begin();
  for (; it != _node->children->end(); ++it) {
    CETNode* const childNode = it->second;
    if (contains(_transaction, childNode->itemset, false)) {
      {
        if (childNode->tidlist->size() == 0) {
          exit(ERROR_DELETE_NODE_WITH_EMPTY_TIDSET);
        }
        if (childNode->tidlist->size() != 0 && childNode->tidlist->at(0) == _tid) {
          childNode->support -= 1;
          childNode->tidsum -= _tid;
          childNode->oldHash = childNode->hash;
          if (childNode->oldHash == 0) {
            exit(ERROR_DELETE_NODE_WITH_HASH);
          }
          childNode->hash = childNode->tidsum * childNode->support;
          std::vector<uint32_t>* const tmpTidList = new std::vector<uint32_t>(childNode->tidlist->begin() + 1, childNode->tidlist->end());
          //final int[] tmpTidList = new int[childNode.tidlist.length - 1];
          //System.arraycopy(childNode.tidlist, 1, tmpTidList, 0, childNode.tidlist.length - 1);
          childNode->tidlist = tmpTidList;
        }
        else {
          if (binary_search(childNode->tidlist->begin(), childNode->tidlist->end(), _tid)) {
            exit(ERROR_DELETE_NODE_CONTAINS_TRX_BUT_NOT_IN_TIDSET);
          }
        }
      }
      //si on utilise pas, delete ceci
      if (childNode->support < _minsupp) {
        infrequent_nodes.push_back(it->first);
      }

      //on pourrait peut etre remplacer la seconde condition par type != INFREQUENT_GATEWAY ?
      if (childNode->support < _minsupp && (childNode->support + 1) >= _minsupp) {
        //newly infrequent
        newly_infrequent_nodes.push_back(it->first);
        if (childNode->type == CLOSED_NODE) {
          delete_ci(childNode, _EQ_TABLE);
        }
        //prune ses descendants de l'arbre
        prune_children(childNode, _tid, _EQ_TABLE);
        //marquer comme infrequent gateway
        childNode->type = INFREQUENT_GATEWAY_NODE;
      }
      else if (childNode->type == UNPROMISSING_GATEWAY_NODE || childNode->type == INFREQUENT_GATEWAY_NODE) {
        //verifier le WAS unpromissing or infrequent
        prune_children(childNode, _tid, _EQ_TABLE);
      }
      else if (left_check(childNode, _EQ_TABLE)) {
        if (childNode->type == CLOSED_NODE) {
          delete_ci(childNode, _EQ_TABLE);
        }
        prune_children(childNode, _tid, _EQ_TABLE);
        childNode->type = UNPROMISSING_GATEWAY_NODE;
      }
      else {
        //ici probablement newly infrequent only ?
        std::vector<uint32_t>::iterator infrequent_sib_it = newly_infrequent_nodes.begin();
        for (; infrequent_sib_it != newly_infrequent_nodes.end(); ++infrequent_sib_it) {
          if (*infrequent_sib_it > childNode->item) {
            if (childNode->children->end() != childNode->children->find(*infrequent_sib_it)) {
              CETNode* const childNodeObs = childNode->children->find(*infrequent_sib_it)->second;
              prune_children(childNodeObs, _tid, _EQ_TABLE);
              childNode->children->erase(*infrequent_sib_it);//prune superset of infrequent gateway node
              //ceci ne devrait jamais arriver ? si c'est des fermes qui sont infrequents
              if (childNodeObs->type == CLOSED_NODE) {
                exit(ERROR_DELETE_REMOVING_INFREQUENT_CI);
              }
              NBR_NODES -= 1;
            }
          }
        }
        //System.out.println("starting recursion for "+Arrays.toString(childNode.itemset));
        Deletion(_tid, _transaction, _minsupp, childNode, _EQ_TABLE);
        if (childNode->type == CLOSED_NODE) {
          //test les fils support
          bool has_child_with_same_support = false;
          if (childNode->children) {

            std::map<uint32_t, CETNode*>::iterator it = childNode->children->begin();
            for (; it != childNode->children->end(); ++it) {
              CETNode* const ni_second = it->second;
              if (ni_second->support == childNode->support) {
                has_child_with_same_support = true;
                break;
              }
            }
          }
          if (has_child_with_same_support) {
            delete_ci(childNode, _EQ_TABLE);
            childNode->type = INTERMEDIATE_NODE;
            //prune_children(childNode, _tid, _EQ_TABLE);
          }
          else {
            //System.out.println("ok still closed "+Arrays.toString(childNode.itemset));
            //update la table de hashage
            if (childNode->support == 0) {
              prune_children(childNode, _tid, _EQ_TABLE);
              delete_ci(childNode, _EQ_TABLE);
              _node->children->erase(it->first);
            }
            else {
              update_cetnode_in_hashmap(childNode, _EQ_TABLE);
            }
          }
        }
      }
    }
  }
};

std::vector<uint32_t>* left_check(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (_EQ_TABLE->end() != _EQ_TABLE->find(_node->hash)) {
    std::vector<std::vector<CETNode*>*>* const potential_closures_by_size = _EQ_TABLE->find(_node->hash)->second;
    for (int i = _node->itemset->size() + 1; i < potential_closures_by_size->size(); ++i) {
      std::vector<CETNode*>* const strate = potential_closures_by_size->at(i);
      std::vector<CETNode*>::iterator it = strate->begin();
      for (; it != strate->end(); ++it) {
        CETNode* const candidate = *it;
        if (candidate->support == _node->support && contains(candidate->itemset, _node->itemset, true)) {
          //il faut aussi tester que le candidat a la branche de fermeture contient des items lexicographiquement plus petits que ceux dans le noeud a tester
          std::vector<uint32_t>* const set_difference = diff(candidate->itemset, _node->itemset);
          if (_node->itemset->size() == 0) return set_difference;
          for (int j = 0; j != set_difference->size(); ++j) {
            if (set_difference->at(j) < _node->itemset->at(_node->itemset->size() - 1)) {
              //if(_print) System.out.println("set diff "+Arrays.toString(set_difference)+" and node was "+Arrays.toString(_node.itemset)+" and other CI was "+Arrays.toString(candidate.itemset));
              return set_difference;
            }
          }
        }
      }
    }
  }
  return 0;
};

void add_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (NBR_CLOSED_NODES != CLOSED_ITEMSETS.size()) {
    //System.out.println("before add erreur d'integrite " + NBR_CLOSED_NODES + " vs " + CLOSED_ITEMSETS.size());
    //System.exit(1);
  }

  if (CLOSED_ITEMSETS.end() != CLOSED_ITEMSETS.find(_node->id)) {
    //System.out.println("on a deja cet id #" + _node.id + " " + Arrays.toString(CLOSED_ITEMSETS.get(_node.id).itemset) + " vs " + Arrays.toString(_node.itemset));
    //System.exit(1);
  }

  CLOSED_ITEMSETS.emplace(_node->id, _node);
  NBR_CLOSED_NODES += 1;

  if (NBR_CLOSED_NODES != CLOSED_ITEMSETS.size()) {
    //System.out.println("after add erreur d'integrite " + NBR_CLOSED_NODES + " vs " + CLOSED_ITEMSETS.size());
    //System.exit(1);
  }

};

void delete_ci(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  //System.out.println("deleting "+Arrays.toString(_node.itemset)+" w/ "+_node.support);
  if (NBR_CLOSED_NODES != CLOSED_ITEMSETS.size()) {
    //System.out.println("before erreur d'integrite " + NBR_CLOSED_NODES + " vs " + CLOSED_ITEMSETS.size());
    //System.exit(1);
  }

  NBR_CLOSED_NODES -= 1;
  if (CLOSED_ITEMSETS.find(_node->id) == CLOSED_ITEMSETS.end()) {
    //System.out.println("(delete) oh shit, " + _node.id + " should " + Arrays.toString(_node.itemset) + " be here..." + _node.support);
    //new Throwable().printStackTrace(System.out);
    //System.exit(1);
  }

  CLOSED_ITEMSETS.erase(_node->id);
  //_node.oldHash = _node.hash;
  remove_from_class(_node, _EQ_TABLE);

  //_node.parent.children.remove(_node.item);

  if (NBR_CLOSED_NODES != CLOSED_ITEMSETS.size()) {
    //System.out.println("erreur d'integrite " + NBR_CLOSED_NODES + " vs " + CLOSED_ITEMSETS.size());
    //System.exit(1);
  }
};

void prune_children(CETNode* const _node, const uint32_t _tid, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  if (_node->children) {
    //pour chaque fils call prune_children 
    //check les fils, si closed remove dans la table
    //aussi remvove de CLOSED_ITEMSETS
    std::map<uint32_t, CETNode*>::iterator it = _node->children->begin();
    for (; it != _node->children->end(); ++it) {
      CETNode* const node = it->second;
      if (node->type == CLOSED_NODE) {
        delete_ci(node, _EQ_TABLE);
      }
      prune_children(node, _tid, _EQ_TABLE);
      NBR_NODES -= 1;
    }
    _node->children->clear();
  }
};

void remove_from_class(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  {
    if (_EQ_TABLE->find(_node->oldHash) == _EQ_TABLE->end() || !_EQ_TABLE->find(_node->oldHash)->second) {
      _node->oldHash = _node->hash;
    }
    std::vector<CETNode*>* const ancienneClasse = _EQ_TABLE->find(_node->oldHash)->second->at(_node->itemset->size());
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
      std::vector<CETNode*>* const ancienneClasse2 = _EQ_TABLE->find(_node->hash)->second->at(_node->itemset->size());
      uint32_t positionMatch2 = ancienneClasse2->size();
      uint32_t cursor2 = 0;
      std::vector<CETNode*>::iterator it2 = ancienneClasse2->begin();
      for (; it2 != ancienneClasse2->end(); ++it2) {
        CETNode* const n = *it2;
        if (exactMatch(n->itemset, _node->itemset)) {
          positionMatch2 = cursor2;//ok trouve, on doit le supprimer
          break;
        }
        cursor2 += 1;
      }
      if (positionMatch2 != ancienneClasse2->size()) {
        //System.out.println("but was found in "+_node.hash+" !!!!");
        ancienneClasse2->erase(ancienneClasse2->begin() + positionMatch2);
      }
      else {
        //System.out.println("could not be found...");
        //System.out.println((_called_from ? "(rem)" : "(add)") + " could not find CI to transfer between classes..." + Arrays.toString(_node.itemset) + " has a support of s=" + _node.support + " hash=" + _node.hash + " oldHash=" + _node.oldHash + " tidset=" + Arrays.toString(_node.tidlist));
        //System.exit(1);
      }
      //System.exit(1);
    }
  }
};

void update_cetnode_in_hashmap(CETNode* const _node, std::map<long, std::vector<std::vector<CETNode*>*>*>* const _EQ_TABLE) {
  remove_from_class(_node, _EQ_TABLE);
  //add into new class
  //on ajoute le noeud dans la bonne classe d'equivalence (tidsum + support)
  {
    if (_EQ_TABLE->end() == _EQ_TABLE->find(_node->hash)) {
      //_EQ_TABLE->emplace(_node->hash, new std::vector<CETNode*>());
    }
    if (_EQ_TABLE->find(_node->hash)->second->size() <= _node->itemset->size()) {
      while (_EQ_TABLE->find(_node->hash)->second->size() <= _node->itemset->size()) {
        _EQ_TABLE->find(_node->hash)->second->push_back(new std::vector<CETNode*>());
      }
    }
    _EQ_TABLE->find(_node->hash)->second->at(_node->itemset->size())->push_back(_node);
  }
};