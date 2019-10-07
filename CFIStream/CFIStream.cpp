#include "CFIStream.h"


void Addition(DUINode* const _root, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _new_cis) {
  DUINode* ref;
  //un peu stupide, autant sortir directement ? un subset de modifie sera tjrs modifie...
  if ((ref = is_in_tree(_transaction, _root))) {
    if (X_0 && _transaction->size() == X_0->size()) {
      //on pourrait aussi verifier avec ref.itemset.length == X_0.length
      ref->count += 1;//incremente le compteur de transactions (utile pour le delete)
    }
    //enumere et incremente les subsets stricts
    enumerate_subsets(ref, INCREMENT, false);
    if (X_close) {
      return;
    }
  }
  else {
    uint32_t supportVal;
    if ((supportVal = support(_transaction, _root)) > 0) {
      if (_new_cis->empty()) {
        //ici il s´agit forcement du ci transaction car on fait du bottom up, il est tjrs le premier
        X_0 = _transaction;//copy
        DUINode* const newci = new DUINode();
        newci->itemset = _transaction;
        _new_cis->push_back(newci);
        X_close = false;
        uint32_t X_s = /*support(_transaction, _root)*/supportVal + 1;
        newci->support = X_s;
      }
      else {
        std::vector<DUINode*> superset_cis = std::vector<DUINode*>();
        enumerate_supersets(_transaction, &superset_cis);
        //recherche du geniteur, verifier qu´il y a au moins 1
        DUINode* M = superset_cis.at(0);
        //faire une autre methode find_genitor (profiling hotspots)
        for (int i = 1; i != superset_cis.size(); ++i) {
          DUINode* node = superset_cis.at(i);
          if (node->itemset->size() < M->itemset->size()) {
            M = node;
          }
        }
        std::vector<uint32_t>* X_c = M->itemset;//copy ?
        std::vector<uint32_t>* diff_genitor_trx = diff(X_c, _transaction);
        std::vector<uint32_t>* inter2 = inter(diff_genitor_trx, X_0);//on filtre avec les items de la transaction uniquement
        if (inter2->size() == 0 && X_c->size() != 0) {
          //deuxieme condition est tjrs vraie
          DUINode* const newci = new DUINode();
          newci->itemset = _transaction;
          _new_cis->push_back(newci);
          uint32_t X_s = /*support(_transaction, _root)*/supportVal + 1;//Ceci est un peu stupide... normalement c´est M.support + 1
          //ajouter le nouveau dans le DUI Tree
          newci->support = X_s;
        }
      }

    }
    else {
      if (_new_cis->empty()) {
        //ici il s´agit forcement du ci transaction car on fait du bottom up, il est tjrs le premier
        X_0 = _transaction;//copy
        DUINode* const newci = new DUINode();
        newci->itemset = _transaction;
        _new_cis->push_back(newci);
        X_close = false;
        uint32_t X_s = 1;
        newci->count = X_s;
        newci->support = X_s;
      }
    }

  }
  //forall subsets of _transaction (This is where the FUN begins ^^)
  //faire une autre methode (profiling)
  std::vector<std::vector<uint32_t>>* const all_direct_subsets = new std::vector<std::vector<uint32_t>>();
  if (_transaction->size() > 1) {
    for (uint32_t i = 0; i != _transaction->size(); ++i) {
      std::vector<uint32_t>* const subset = &std::vector<uint32_t>();// [_transaction.length - 1];
      for (uint32_t j = 0; j != _transaction->size(); ++j) {
        if(i != j) subset->push_back(_transaction->at(j));
      }
      all_direct_subsets->push_back(*subset);
    }
  }
  for (uint32_t i = 0; i != all_direct_subsets->size(); ++i) {
    uint32_t candidate_hash = 0;
    std::vector<uint32_t>* const candidate = &all_direct_subsets->at(i);
    for (uint32_t j = 0; j != candidate->size(); ++j) {
      candidate_hash += candidate->at(j);
    }
    bool has_already_been_enumerated = false;
    if (enumerations.find(candidate_hash) != enumerations.end()) {
      std::vector<std::vector<uint32_t>>* const already_emumerated_subsets = &enumerations.find(candidate_hash)->second;
      std::vector<std::vector<uint32_t>>::iterator it = already_emumerated_subsets->begin();
      for(; it != already_emumerated_subsets->end(); ++it){
        std::vector<uint32_t>* const old = &(*it);
        if (exactMatch(old, candidate)) {
          has_already_been_enumerated = true;
          break;
        }
      }
    }
    else {
      enumerations.emplace(candidate_hash, std::vector<uint32_t>());
    }
    if (!has_already_been_enumerated) {
      (&enumerations.find(candidate_hash)->second)->push_back(*candidate);//ATTENTION copy
      TOTAL_CANDIDATES += 1;
      Addition(_root, &all_direct_subsets->at(i), _new_cis);
    }
  }
};


void enumerate_supersets(std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _superset_cis) {
  //comment les strates vont intervenir ici ?
  std::map<uint32_t, std::vector<DUINode*>>::iterator it = HEADER_STRATE.begin();
  for (; it != HEADER_STRATE.end(); ++it) {
    if (it->first <= _transaction->size()) {
      continue;//skip if level to large
    }
    std::vector<DUINode*>* const strate = &it->second;
    std::vector<DUINode*>::iterator strate_it = strate->begin();
    for (; strate_it != strate->end(); ++strate_it) {
      DUINode* const n = *strate_it;
      if (contains(n->itemset, _transaction, true)) {
        //un subset
        _superset_cis->push_back(n);
      }
    }
  }
}

void enumerate_subsets(DUINode* const _ref, const uint8_t _action, const bool _strict) {
  //here +1 if incrment or -1 if decrement
  std::vector<DUINode*> all_subsets = std::vector<DUINode*>();
  if (!_strict) {
    all_subsets.push_back(_ref);
  }
  std::map<uint32_t, std::vector<DUINode*>>::reverse_iterator it = HEADER_STRATE.rbegin();
  for (; it != HEADER_STRATE.rend(); --it) {
    if (it->first >= _ref->itemset->size()) {
      continue;//skip if level to large
    }
    std::vector<DUINode*>* const strate = &it->second;
    std::vector<DUINode*>::iterator strate_it = strate->begin();
    for (; strate_it != strate->end(); ++strate_it) {
      DUINode* const n = *strate_it;
      if (contains(_ref->itemset, n->itemset, true)) {
        all_subsets.push_back(n);
      }
    }
  }
  std::vector<DUINode*>::iterator all_subsets_it = all_subsets.begin();
  for (; all_subsets_it != all_subsets.end(); ++all_subsets_it) {
    DUINode* const n = *all_subsets_it;
    switch (_action) {
    case DECREMENT:
      n->support -= 1;
      break;
    case INCREMENT:
      n->support += 1;
      break;
    default:
      break;
    }
  }
};

DUINode* is_in_tree(std::vector<uint32_t>* const _transaction, DUINode* const _root) {
  DUINode* curr = _root;
  for (int i = 0; i != _transaction->size(); ++i) {
    if (curr->children && (curr->children->find(_transaction->at(i)) != curr->children->end())) {
      curr = (curr->children->find(_transaction->at(i)))->second;
    }
    else {
      return 0;
    }
  }
  if (!curr->itemset) return 0;
  return curr;
};

DUINode* count(std::vector<uint32_t>* const _transaction, DUINode* const _node) {
  DUINode* curr = _node;
  uint32_t matched_chars = 0;
  while (matched_chars <= _transaction->size()) {
    std::map<uint32_t, DUINode*>::iterator it = curr->children->begin();
    bool found_next = false;
    for(; it != curr->children->end(); ++it){
    //for (int i = 0; i != keys.length; ++i) {
      DUINode* const child = it->second;
      if (contains(_transaction, child->itemset, false)) {
        curr = child;
        found_next = true;
        matched_chars = curr->itemset->size();
        break;
      }
    }
    if (!found_next) {
      return 0;
    }
  }
  return curr;
};
 
uint32_t support(std::vector<uint32_t>* const _transaction, DUINode* const _node) {
  std::map<uint32_t, std::vector<DUINode*>>::iterator it = HEADER_STRATE.begin();
  for (; it != HEADER_STRATE.end(); ++it) {
    if (it->first >= _transaction->size()) {
      //on commence a tester
      std::vector<DUINode*>* const strate = &it->second;
      if (strate) {
        std::vector<DUINode*>::iterator it2 = strate->begin();
        for (; it2 != strate->end(); ++it2) {
          DUINode* const n = *it2;
          if (contains(n->itemset, _transaction, false)) {
            return n->support;
          }
        }
      }
    }
  }
  return 0;
};


void insert_in_tree(DUINode* const _node, DUINode* const _root) {
  //refaire ceci en bouclant sur les items et verifiant children.
  //si existe pas, alors on l'ajoute. (idem Moment et NewMoment)
  DUINode* curr = _root;
  for (int i = 0; i != _node->itemset->size(); ++i) {
    if (curr->children && curr->children->find(_node->itemset->at(i)) != curr->children->end()) {
      curr = curr->children->find(_node->itemset->at(i))->second;
    }
    else {
      if (!curr->children) {
        curr->children = new std::map<uint32_t, DUINode*>();
      }
      DUINode* const nthitem = new DUINode();
      curr->children->emplace(_node->itemset->at(i), nthitem);
      //no need to fill itemset fields or support
      curr = nthitem;
    }
  }
  curr->itemset = _node->itemset;
  curr->support = _node->support;
  curr->count = _node->count;
};