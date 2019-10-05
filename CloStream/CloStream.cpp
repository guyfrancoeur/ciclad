#include "CloStream.h"


std::set<uint32_t>* compute_union_ideals(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap) {
  std::set<uint32_t>* cidset = new std::set<uint32_t>();
  // For each item in the transaction
  std::vector<uint32_t>::iterator trx_it = _transaction->begin();
  for (; trx_it != _transaction->end(); ++trx_it) {
    const uint32_t item = *trx_it;
    // get the cid list of that item
    std::vector<uint32_t>* const cidlist = &_cidListMap->find(item)->second;
    if (cidlist) {
      // add the cid list to the combined cid list
      std::vector<uint32_t>::iterator it = cidlist->begin();
      for (; it != cidlist->end(); ++it) {
        cidset->emplace(*it);
      }
    }
  }
  return cidset;
};

std::map<CloStreamCI*, uint32_t>::iterator* locate_intersection_class(CloStreamCI* const _candidateGenitor, CloStreamCI* _intersection, std::map<CloStreamCI*, uint32_t>* const _tableTemp) {
  std::map<CloStreamCI*, uint32_t>::iterator it = _tableTemp->begin();
  for (; it != _tableTemp->end(); ++it) {
    // if it is equal to the intersection
    if (exactMatch(it->first->itemset, _intersection->itemset)) {
      return &it;
    }
  }
  return 0;
};

void processNewTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed) {
  // a temporary table (as described in the paper) to 
  // associate itemsets with cids.
  std::map<CloStreamCI*, uint32_t> tableTemp = std::map<CloStreamCI*, uint32_t>();

  // Line 02 of the pseudocode in the article
  // We add the transaction in a temporary table
  CloStreamCI trxci = CloStreamCI();
  trxci.itemset = _transaction;
  trxci.support = 0;
  tableTemp.emplace(&trxci, 0);

  // Line 03  of the pseudocode in the article
  // Create a set to store the combined cidlist of items in the transaction
  std::set<uint32_t>* cidset = compute_union_ideals(_transaction, _cidListMap);
  // Line 04  of the pseudocode in the article
  // For each cid in the combined set of cids
  std::set<uint32_t>::iterator it = cidset->begin();
  for (; it != cidset->end(); ++it) {
    const uint32_t cid = *it;
    if (!cid) continue;
    // Get the closed itemset corresponding to this cid
    CloStreamCI* cti = _tableClosed->find(cid)->second;
    // create the intersection of this closed itemset
    // and the transaction.
    CloStreamCI intersection = CloStreamCI();
    intersection.itemset = inter(cti->itemset, _transaction);

    std::map<CloStreamCI*, uint32_t>::iterator* already_computed_intersection;
    if (!(already_computed_intersection = locate_intersection_class(cti, &intersection, &tableTemp))) {
      // add the instersection to the temporary table with "cid".
      tableTemp.emplace(intersection, cid);
    }
    else {
      CloStreamCI* const ctt = _tableClosed->find((*already_computed_intersection)->second)->second;
      // if the support of cti is higher than ctt
      if (cti->support > ctt->support) {
        // set the value as "cid".
        (*already_computed_intersection)->second = cid;
      }
    }
  }
  // For each entry in the temporary table
  std::map<CloStreamCI*, uint32_t>::iterator it2 = tableTemp.begin();
  for (; it2 != tableTemp.end(); ++it2) {
    // get the itemset
    CloStreamCI* newCI = it2->first;
    // get the cid
    // get the closed itemset for that cid
    CloStreamCI* genitor = _tableClosed->find(it2->second)->second;
    // if the itemset is the same as the closed itemset
    if (exactMatch(newCI->itemset, genitor->itemset)) {
      // we have to increase its support
      genitor->support += 1;
    }
    else {
      // its support count is set to the support of ctc + 1.
      newCI->support = genitor->support + 1;
      newCI->id = CI_ID++;
      newCI->positions_in_lists = new std::vector<uint32_t>();// newCI.itemset.length];
      //System.out.println("added "+Arrays.toString(newCI.itemset));
      // otherwise the itemset "x" is added to the table of closed itemsets
      _tableClosed->emplace(newCI->id, newCI);
      // Finally, we loop over each item in the transaction again                
      for (uint32_t i = 0; i != newCI->itemset->size(); ++i) {
        const uint32_t item = newCI->itemset->at(i);
        // we get the cidlist of the current item
        std::vector<uint32_t>* cidlist = &_cidListMap->find(item)->second;
        // if null
        if (!cidlist) {
          cidlist = new std::vector<uint32_t>();
          // we  create one
          _cidListMap->emplace(item, cidlist);
        }
        // then we add x to the cidlist
        const uint32_t pos = cidlist->size();
        cidlist->push_back(newCI->id);
        newCI->positions_in_lists->push_back(pos);
      }
    }
  }
};

void removeOldTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed) {
  std::map<CloStreamCI*, uint32_t> tableTemp = std::map<CloStreamCI*, uint32_t>();
  CloStreamCI trxci = CloStreamCI();
  trxci.itemset = _transaction;
  trxci.support = 0;
  tableTemp.emplace(&trxci, 0);

  std::set<uint32_t>* cidset = compute_union_ideals(_transaction, _cidListMap);
  std::set<uint32_t>::iterator it = cidset->begin();
  for (; it != cidset->end(); ++it) {
    const uint32_t cid = *it;
    if (!cid) continue;
    CloStreamCI* cti = _tableClosed->find(cid)->second;
    CloStreamCI intersection = CloStreamCI();
    intersection.itemset = inter(cti->itemset, _transaction);

    std::map<CloStreamCI*, uint32_t>::iterator* already_computed_intersection;
    if (!(already_computed_intersection = locate_intersection_class(cti, &intersection, &tableTemp))) {
      if (exactMatch(intersection.itemset, cti->itemset)) {
        tableTemp.emplace(cti, 0);//ici il faut cibler bottom. il le geniteur ne peut pas etre le CI lui meme.
      }
      else {
        //les CI conserves via leurs cid doivent etre a l'exterieur du cone up trx
        if (!contains(_transaction, cti->itemset, false)) {
          tableTemp.emplace(intersection, cid);
        }
      }
    }
    else {
      CloStreamCI* const candGenitor = _tableClosed->find((*already_computed_intersection)->second)->second;
      // if the support of cti is higher than ctt
      //les CI conserves via leurs cid doivent etre a l'exterieur du cone up trx
      if (cti->support > candGenitor->support && !contains(_transaction, cti->itemset, false)) {
        (*already_computed_intersection)->second = cid;// set the value as "cid".
      }
      if (exactMatch(intersection.itemset, cti->itemset)) {
        tableTemp.erase((*already_computed_intersection)->first);
        tableTemp.emplace(cti, (*already_computed_intersection)->second);
      }
    }
  }
  
  std::map<CloStreamCI*, uint32_t>::iterator it2 = tableTemp.begin();
  for (; it2 != tableTemp.end(); ++it2) {
    // get the itemset
    CloStreamCI* const obsoleteOrDemotedCI = it2->first;
    // get the cid
    const uint32_t c = it2->second;
    // get the closed itemset for that cid
    CloStreamCI* const genitor = _tableClosed->find(c)->second;
    //
    if (genitor->support == (obsoleteOrDemotedCI->support - 1) /*&& LatticeExport.contains(_transaction, genitor.itemset, false) != 0x01*/) {
      //System.out.println("removed "+Arrays.toString(obsoleteOrDemotedCI.itemset)+" with "+(obsoleteOrDemotedCI.support - 1));
      for (int i = 0; i != obsoleteOrDemotedCI->positions_in_lists->size(); ++i) {
        std::vector<uint32_t>* cidlist = &_cidListMap->find(obsoleteOrDemotedCI->itemset->at(i))->second;
        if (!cidlist) {
          System.out.println("error, this cannot happen");
          System.exit(1);
        }
        cidlist->erase(cidlist->begin() + obsoleteOrDemotedCI->positions_in_lists->at(i));
        cidlist->at(obsoleteOrDemotedCI->positions_in_lists->at(i)) = 0;
      }
      _tableClosed->erase(obsoleteOrDemotedCI->id);
      _tableClosed->emplace(obsoleteOrDemotedCI->id, 0);
    }
    else {
      // we have to increase its support
      obsoleteOrDemotedCI->support -= 1;
    }
  }
};