#include "CFIStream.h"

extern uint32_t NBR_CLOSED_NODES;
//std::map<uint32_t, DUINode*> CLOSED_ITEMSETS;

extern std::map<uint32_t, std::vector<DUINode*>> HEADER_STRATE;
extern std::map<uint32_t, DUINode*> HEADER_MIN;


extern std::map<uint32_t, std::vector<std::vector<uint32_t>>> enumerations;
extern uint32_t TOTAL_CANDIDATES;

const uint8_t INCREMENT = 0x01;
const uint8_t DECREMENT = 0x02;

extern bool X_close;//Il semble que cela serve uniquement a s arreter lorsque la transaction est deja un CI ???
extern std::vector<uint32_t>* X_0;

void Addition(DUINode* const _root, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _new_cis) {
  DUINode* ref;
  //un peu stupide, autant sortir directement ? un subset de modifie sera tjrs modifie...
  if ((ref = is_in_tree(_transaction, _root))) {
    if (!X_0) {
      ref->count += 1;//incremente le compteur de transactions (utile pour le delete)
    }
    /*if (X_0 && _transaction->size() == X_0->size()) {
      //on pourrait aussi verifier avec ref.itemset.length == X_0.length
      ref->count += 1;//incremente le compteur de transactions (utile pour le delete)
    }*/
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
        X_0 = new std::vector<uint32_t>(_transaction->begin(), _transaction->end());//copy
        DUINode* const newci = new DUINode();
        newci->itemset = X_0;
        _new_cis->push_back(newci);
        X_close = false;
        uint32_t X_s = /*support(_transaction, _root)*/supportVal + 1;
        newci->support = X_s;
        newci->count = 1;
      }
      else {
        std::vector<DUINode*> superset_cis = std::vector<DUINode*>();
        enumerate_supersets(_transaction, &superset_cis);
        //recherche du geniteur, verifier qu´il y a au moins 1
        DUINode* M = superset_cis.at(0);
        //faire une autre methode find_genitor (profiling hotspots)
        for (uint32_t i = 1; i != superset_cis.size(); ++i) {
          DUINode* node = superset_cis.at(i);
          if (node->itemset->size() < M->itemset->size()) {
            M = node;
          }
        }
        std::vector<uint32_t>* const X_c = M->itemset;//copy ?
        std::vector<uint32_t>* diff_genitor_trx = diff(X_c, _transaction);
        std::vector<uint32_t>* inter2 = inter(diff_genitor_trx, X_0);//on filtre avec les items de la transaction uniquement
        if (inter2->size() == 0 && X_c->size() != 0) {
          //deuxieme condition est tjrs vraie
          DUINode* const newci = new DUINode();
          newci->itemset = new std::vector<uint32_t>(_transaction->begin(), _transaction->end());//copy
          _new_cis->push_back(newci);
          uint32_t X_s = /*support(_transaction, _root)*/supportVal + 1;//Ceci est un peu stupide... normalement c´est M.support + 1
          //ajouter le nouveau dans le DUI Tree
          newci->support = X_s;
          newci->count = 0;
        }
      }
    }
    else {
      if (_new_cis->empty()) {
        //ici il s´agit forcement du ci transaction car on fait du bottom up, il est tjrs le premier
        X_0 = new std::vector<uint32_t>(_transaction->begin(), _transaction->end());//copy
        DUINode* const newci = new DUINode();
        newci->itemset = X_0;
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
      enumerations.emplace(candidate_hash, std::vector<std::vector<uint32_t>>());
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
      if (n->flaged_for_removal) continue;
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
  for (; it != HEADER_STRATE.rend(); ++it) {
    if (it->first >= _ref->itemset->size()) {
      continue;//skip if level to large
    }
    std::vector<DUINode*>* const strate = &it->second;
    std::vector<DUINode*>::iterator strate_it = strate->begin();
    for (; strate_it != strate->end(); ++strate_it) {
      DUINode* const n = *strate_it;
      if (n->flaged_for_removal) continue;
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
  for (uint32_t i = 0; i != _transaction->size(); ++i) {
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
  std::map<uint32_t, std::vector<DUINode*>>::iterator existing_strata = HEADER_STRATE.begin();
  for (; existing_strata != HEADER_STRATE.end(); ++existing_strata) {
    if (existing_strata->first == _transaction->size()) {
      //on commence a tester
      std::vector<DUINode*>* const strate = &existing_strata->second;
      if (strate) {
        std::vector<DUINode*>::iterator it = strate->begin();
        for(; it != strate->end(); ++it){
        //for (final DUINode n : strate) {
          DUINode* const n = *it;
          if (n->flaged_for_removal) continue;
          if (contains(n->itemset, _transaction, false)) {
            return n;
          }
        }
      }
    }
  }
  return 0;
}
 
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
            //print_array(n->itemset);
            //print_array(_transaction);
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
  for (uint32_t i = 0; i != _node->itemset->size(); ++i) {
    if (curr->children && curr->children->find(_node->itemset->at(i)) != curr->children->end()) {
      curr = curr->children->find(_node->itemset->at(i))->second;
    }
    else {
      if (!curr->children) {
        curr->children = new std::map<uint32_t, DUINode*>();
      }
      DUINode* const nthitem = new DUINode();
      nthitem->item = _node->itemset->at(i);
      nthitem->parent = curr;
      curr->children->emplace(_node->itemset->at(i), nthitem);
      //no need to fill itemset fields or support
      curr = nthitem;
    }
  }

  _node->parent = curr->parent;
  _node->item = curr->item;
  _node->children = curr->children;
  curr->parent->children->erase(curr->item);
  curr->parent->children->emplace(curr->item, _node);
  /*curr->itemset = _node->itemset;
  curr->support = _node->support;
  curr->count = _node->count;*/
};

void Suppression(DUINode* const _node, std::vector<uint32_t>* const _transaction, std::vector<DUINode*>* const _obs_cis) {
  DUINode* ref = count(_transaction, _node);
  if (ref->count >= 2) {
    enumerate_subsets(ref, DECREMENT, false);
    ref->count -= 1;
  }
  else {
    std::vector<std::vector<DUINode*>> all_strata;
    enumerate_subsets_stratified(_transaction, &all_strata);
    //il faut tous les generer !!! depuis l'abre, parcourir les strates !!!!
    uint32_t len = _transaction->size();
    std::vector<DUINode*> trx_strata = std::vector<DUINode*>();
    trx_strata.push_back(ref);
    all_strata.push_back(trx_strata);
    while (len >= 1) {
      //recuperer tous les subsets de _transaction d´une taille specifique
      std::vector<DUINode*>* const subsets_in_strata = &all_strata.at(len);
      if (!subsets_in_strata) continue;
      for (uint32_t j = 0; j != subsets_in_strata->size(); ++j) {
        DUINode* Y_node = subsets_in_strata->at(j);
        std::vector<uint32_t>* const Y = Y_node->itemset;
        //if(Y_node.count != 1 && Y.length != _transaction.length){
            //une trx qui n'est pas celle actuelle
        if (Y_node->count >= 2) {
          //System.out.println("" + Arrays.toString(Y) + " will not disapear because " + Y_node.count);
          Y_node->support -= 1;//decrement
          //si match trx alors aussi decrement le count
          if (exactMatch(_transaction, Y)) {
            Y_node->count -= 1;
          }
        }
        else if (Y_node->count == 1 && !exactMatch(_transaction, Y)) {
          //il apparait une fois, mais ce n'est pas la trx a supprimer
          //comment faire ? 
          //si il a des supersets qui flag pour le delete ?
          //comaprer le support ?
          //dans ce cas, il n'est PAS obsolete
          Y_node->support -= 1;//decrement
          //
        }
        else {
          //si leur count >= 2
          std::vector<DUINode*> superset_cis = std::vector<DUINode*>();
          //System.out.println("----");
          enumerate_supersets(Y, &superset_cis);
          //std::cout << superset_cis.size() << std::endl;
          //superset_cis.add(ref);
          std::vector<uint32_t>* M = 0;
          //System.out.println("we have " + superset_cis.size() + " supersetsfor s=" + Y_node.support + " " + Y_node.count);
          if (!superset_cis.empty()) {
            M = superset_cis.at(0)->itemset;
            //System.out.println("supp=" + superset_cis.get(0).support);
            std::vector<uint32_t>* oldRef = 0;
            for (uint32_t i = 1; i != superset_cis.size(); ++i) {
              M = inter(M, superset_cis.at(i)->itemset);
              if (oldRef != 0) {
                delete oldRef;
              }
              oldRef = M;
              //System.out.println("supp=" + superset_cis.get(i).support);
            }
            /*if (oldRef != 0) {
              delete oldRef;
            }*/
          }
          //Y est une intersection donc il ne disparait pas... en vrai, dans la boucle du dessus, des qu`on a Y, on peut sortir
          //on peut meme checker la taille uniquement ?
          //System.out.println("M=" + Arrays.toString(M) + " for " + Arrays.toString(Y));
          if (M && exactMatch(M, Y)) {
            Y_node->support -= 1;//decrerment
            if (exactMatch(_transaction, Y)) {
              Y_node->count -= 1;
              //ceci ne devrait pas arriver.... jamais
              //System.exit(1);
              /*final DUINode obsolete = Y_node;
              _obs_cis.add(obsolete);
              Y_node.flaged_for_removal = true;//if not this, we will use obsolete CIs above ..
              */
            }
          }
          else {
            DUINode* const obsolete = Y_node;
            _obs_cis->push_back(obsolete);
            Y_node->flaged_for_removal = true;//if not this, we will use obsolete CIs above ..
          }
          //if (M != 0) delete M;
        }
      }
      len -= 1;
    }
    //delete all_strata;
  }
};

void remove_from_tree(DUINode* const _node, DUINode* const _root) {
  //refaire ceci en bouclant sur les items et verifiant children.
 //si existe pas, alors on l'ajoute. (idem Moment et NewMoment)
  DUINode* curr = _root;
  for (uint32_t i = 0; i != _node->itemset->size(); ++i) {
    if (curr->children && curr->children->find(_node->itemset->at(i)) != curr->children->end()) {
      curr = curr->children->at(_node->itemset->at(i));
    }
    else {
      //System.out.println("error, missing node");
      exit(1);
    }
  }

  if (!curr->children || curr->children->empty()) {
    curr->parent->children->erase(curr->item);//delete node completely
    delete curr->itemset;
    delete curr->children;
    delete curr;
  }
  else {
    curr->itemset = 0;
    curr->support = 0;
    curr->count = 0;
    curr->flaged_for_removal = true;
    //curr.positionInStrate = 0;
  }
}

void enumerate_subsets_stratified(std::vector<uint32_t>* const _transaction, 
  std::vector<std::vector<DUINode*>>* const _all_subsets_stratified) {
  //here +1 if incrment or -1 if decrement
  //std::vector<std::vector<DUINode*>>* all_subsets_stratified = new std::vector<std::vector<DUINode*>>();
  _all_subsets_stratified->resize(_transaction->size());
  /*for (uint32_t i = 0; i != _transaction->size(); ++i) {
    _all_subsets_stratified->push_back(std::vector<DUINode*>());
  }*/
  std::map<uint32_t, std::vector<DUINode*>>::iterator it = HEADER_STRATE.begin();
  for(; it != HEADER_STRATE.end(); ++it){
  //for (int i = _transaction.length; i != 0; --i) {
    if (it->first >= _transaction->size()) {
      continue;//skip if level to large
    }
    std::vector<DUINode*> strate = it->second;
    std::vector<DUINode*>::iterator it2 = strate.begin();
    for(; it2 != strate.end(); ++it2){
      DUINode* const n = *it2;
    //for (final DUINode n : strate) {
      if (n->flaged_for_removal) continue;
      if (contains(_transaction, n->itemset, true)) {
        _all_subsets_stratified->at(it->first).push_back(n);
      }
    }
  }
  //std::cout << _all_subsets_stratified->size() << std::endl;
  //return all_subsets_stratified;
}