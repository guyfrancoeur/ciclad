#include "CloStream.h"
#include "../CommonUtility/Utility.h"

uint32_t CI_ID = 1;

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

uint32_t locate_intersection_class(CloStreamCI* const _candidateGenitor, CloStreamCI* _intersection, std::vector<std::pair<CloStreamCI*, uint32_t>>* const _tableTemp) {
  //std::vector<std::pair<CloStreamCI*, uint32_t>>::iterator it = _tableTemp->begin();

  //uint32_t temp;
  //uint32_t matches = 0;

  for (uint32_t i = 0; i != _tableTemp->size(); ++i) {
    // if it is equal to the intersection
    if (exactMatch((&_tableTemp->at(i))->first->itemset, _intersection->itemset)) {
      return i;
      //temp = i;
      //matches += 1;

      //if (matches > 1) {
      //  std::cout << " more than one match ! " << matches << std::endl;
      //  print_array((&_tableTemp->at(i))->first->itemset);
      //  print_array(_intersection->itemset);
      //  std::cout << "==== INTERSECTION TABLE ======" << std::endl;
      //  for (uint32_t j = 0; j != _tableTemp->size(); ++j) {
      //  std::cout << (&_tableTemp->at(j))->second << " ";
      //    print_array((&_tableTemp->at(j))->first->itemset);
      //  }
      //  std::cout << "==== END INTERSECTION TABLE ======" << std::endl;
      //  exit(1);
      //}
      //return it;
    }
  }
  /*if (matches != 0) {
    return temp;
  }*/
  return _tableTemp->size();
};

void processNewTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed) {
  // a temporary table (as described in the paper) to 
  // associate itemsets with cids.
  std::vector<std::pair<CloStreamCI*, uint32_t>> tableTemp = std::vector<std::pair<CloStreamCI*, uint32_t>>();
  // Line 02 of the pseudocode in the article
  // We add the transaction in a temporary table
  CloStreamCI* const trxci = new CloStreamCI();
  trxci->itemset = new std::vector<uint32_t>(_transaction->begin(), _transaction->end());
  trxci->support = 0;
  tableTemp.push_back(std::pair<CloStreamCI*, uint32_t>(trxci, 0));

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
    CloStreamCI* const intersection = new CloStreamCI();
    intersection->itemset = inter(cti->itemset, _transaction);
    uint32_t already_computed_intersection;
    if (tableTemp.size() == (already_computed_intersection = locate_intersection_class(cti, intersection, &tableTemp))) {
      // add the instersection to the temporary table with "cid".
      tableTemp.push_back(std::pair<CloStreamCI*, uint32_t>(intersection, cid));
    }
    else {
      delete intersection->itemset;
      delete intersection;
      CloStreamCI* const ctt = _tableClosed->find((&tableTemp.at(already_computed_intersection))->second)->second;
      // if the support of cti is higher than ctt
      if (cti->support > ctt->support) {
        // set the value as "cid".
        (&tableTemp.at(already_computed_intersection))->second = cid;
      }
    }
  }
  delete cidset;

  // For each entry in the temporary table
  for (uint32_t i = 0; i != tableTemp.size(); ++i) {
    std::pair<CloStreamCI*, uint32_t>* const it2 = &tableTemp.at(i);
  //std::map<CloStreamCI*, uint32_t>::iterator it2 = tableTemp.begin();
  //for (; it2 != tableTemp.end(); ++it2) {
    // get the itemset
    CloStreamCI* const newCI = it2->first;
    // get the cid
    // get the closed itemset for that cid
    CloStreamCI* const genitor = _tableClosed->find(it2->second)->second;
    // if the itemset is the same as the closed itemset
    if (exactMatch(newCI->itemset, genitor->itemset)) {
      // we have to increase its support
      genitor->support += 1;
      delete newCI->itemset;
      delete newCI;
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
        std::vector<uint32_t>* cidlist;
        if (_cidListMap->find(item) == _cidListMap->end()) {
          // we  create one
          _cidListMap->emplace(item, std::vector<uint32_t>());
        }
        cidlist = &_cidListMap->find(item)->second;
        // then we add x to the cidlist
        const uint32_t pos = cidlist->size();
        cidlist->push_back(newCI->id);
        newCI->positions_in_lists->push_back(pos);
      }
    }
    //delete newCI->itemset;
    //delete newCI;
  }
  //delete trxci->itemset;
  //delete trxci;
};

void removeOldTransaction(std::vector<uint32_t>* const _transaction, std::map<uint32_t, std::vector<uint32_t>>* const _cidListMap, std::map<uint32_t, CloStreamCI*>* const _tableClosed) {
  std::vector<std::pair<CloStreamCI*, uint32_t>> tableTemp = std::vector<std::pair<CloStreamCI*, uint32_t>>();
  CloStreamCI* const trxci = new CloStreamCI();
  trxci->itemset = new std::vector<uint32_t>(_transaction->begin(), _transaction->end());
  trxci->support = 0;
  tableTemp.push_back(std::pair<CloStreamCI*, uint32_t>(trxci, 0));

  std::set<uint32_t>* cidset = compute_union_ideals(_transaction, _cidListMap);
  std::set<uint32_t>::iterator it = cidset->begin();
  for (; it != cidset->end(); ++it) {
    const uint32_t cid = *it;
    if (!cid) continue;
    CloStreamCI* const cti = _tableClosed->find(cid)->second;
    CloStreamCI* const intersection = new CloStreamCI();
    intersection->itemset = inter(cti->itemset, _transaction);

    uint32_t already_computed_intersection;
    if (tableTemp.size() == (already_computed_intersection = locate_intersection_class(cti, intersection, &tableTemp))) {
    //if (!(already_computed_intersection = locate_intersection_class(cti, &intersection, &tableTemp))) {
      if (exactMatch(intersection->itemset, cti->itemset)) {
        //std::cout << "added (b) ";
        //print_array(cti->itemset);
        tableTemp.push_back(std::pair<CloStreamCI*, uint32_t>(cti, 0));//ici il faut cibler bottom. il le geniteur ne peut pas etre le CI lui meme.
        delete intersection->itemset;
        delete intersection;
        /*{
          std::cout << "==== INTERSECTION TABLE ======" << std::endl;
          for (uint32_t j = 0; j != tableTemp.size(); ++j) {
            std::cout << (&tableTemp.at(j))->second << " ";
            print_array((&tableTemp.at(j))->first->itemset);
          }
          std::cout << "==== END INTERSECTION TABLE ======" << std::endl;
        }*/
      }
      else {
        //les CI conserves via leurs cid doivent etre a l'exterieur du cone up trx
        if (!contains(_transaction, cti->itemset, false)) {
          //std::cout << "added (a) ";
          //print_array(intersection->itemset);
          tableTemp.push_back(std::pair<CloStreamCI*, uint32_t>(intersection, cid));
          /*{
            std::cout << "==== INTERSECTION TABLE ======" << std::endl;
            for (uint32_t j = 0; j != tableTemp.size(); ++j) {
              std::cout << (&tableTemp.at(j))->second << " ";
              print_array((&tableTemp.at(j))->first->itemset);
            }
            std::cout << "==== END INTERSECTION TABLE ======" << std::endl;
          }*/
        }
        else {
          delete intersection->itemset;
          delete intersection;
        }
      }
    }
    else {
      CloStreamCI* const candGenitor = _tableClosed->find((&tableTemp.at(already_computed_intersection))->second)->second;
      // if the support of cti is higher than ctt
      //les CI conserves via leurs cid doivent etre a l'exterieur du cone up trx
      if (cti->support > candGenitor->support && !contains(_transaction, cti->itemset, false)) {
        (&tableTemp.at(already_computed_intersection))->second = cid;// set the value as "cid".
      }
      if (exactMatch(intersection->itemset, cti->itemset)) {
        //const uint32_t size1 = tableTemp.size();
        //std::cout << "updated ";
        //print_array((&tableTemp.at(already_computed_intersection))->first->itemset);
        (&tableTemp.at(already_computed_intersection))->first = cti;
        //std::cout << "to ";
        //print_array(cti->itemset);
        /*
        {
          std::cout << "==== INTERSECTION TABLE ======" << std::endl;
          for (uint32_t j = 0; j != tableTemp.size(); ++j) {
            std::cout << (&tableTemp.at(j))->second << " ";
            print_array((&tableTemp.at(j))->first->itemset);
          }
          std::cout << "==== END INTERSECTION TABLE ======" << std::endl;
        }*/

        //tableTemp.erase(already_computed_intersection);
        //tableTemp.erase(already_computed_intersection->first);
        //const uint32_t size2 = tableTemp.size();
        //delete already_computed_intersection->first->itemset;
        //delete already_computed_intersection->first;
        //tableTemp.emplace(cti, (&tableTemp.at(already_computed_intersection))->second);
        /*const uint32_t size3 = tableTemp.size();
        if (size1 == size2 || size2 == size3) {
          exit(1);
        }*/
      }
      delete intersection->itemset;
      delete intersection;
    }
  }
  delete cidset;

  /*std::map<CloStreamCI*, uint32_t>::iterator it22 = tableTemp.begin();
  for (; it22 != tableTemp.end(); ++it22) {
    if (it22->first->id == 0 && it22->first->itemset->size() != _transaction->size()) {
      bool was_found = false;
      std::map<uint32_t, CloStreamCI*>::iterator it3 = _tableClosed->begin();
      for (; it3 != _tableClosed->end(); ++it3) {
        //if (!it3->second) continue;
        if (exactMatch(it3->second->itemset, it22->first->itemset)) {
          was_found = true;
        }
      }
      if (!was_found) {
        std::cout << "could not find some CI"  << std::endl;
        exit(1);
      }
      if (it22->first->support == 0) {
        std::cout << was_found << "but is not in table !" << std::endl;
        exit(1);
      }
    }
  }*/
  
  for (uint32_t i = 0; i != tableTemp.size(); ++i) {
    std::pair<CloStreamCI*, uint32_t>* const it2 = &tableTemp.at(i);
  //std::map<CloStreamCI*, uint32_t>::iterator it2 = tableTemp.begin();
  //for (; it2 != tableTemp.end(); ++it2) {
    // get the itemset
    CloStreamCI* const obsoleteOrDemotedCI = it2->first;
    // get the cid
    const uint32_t c = it2->second;
    // get the closed itemset for that cid
    CloStreamCI* const genitor = _tableClosed->find(c)->second;
    //
    //std::cout << genitor->support << " vs " << obsoleteOrDemotedCI->support << std::endl;
    if (genitor->support == (obsoleteOrDemotedCI->support - 1) /*&& LatticeExport.contains(_transaction, genitor.itemset, false) != 0x01*/) {
      if (!obsoleteOrDemotedCI->positions_in_lists) {
        exit(1);
      }

      //System.out.println("removed "+Arrays.toString(obsoleteOrDemotedCI.itemset)+" with "+(obsoleteOrDemotedCI.support - 1));
      for (uint32_t i = 0; i != obsoleteOrDemotedCI->positions_in_lists->size(); ++i) {
        std::vector<uint32_t>* cidlist = &_cidListMap->find(obsoleteOrDemotedCI->itemset->at(i))->second;
        if (!cidlist) {
          exit(ERROR_TRYING_TO_REMOVE_CI_WITH_NO_CI_LIST);
        }
        cidlist->at(obsoleteOrDemotedCI->positions_in_lists->at(i)) = 0;
      }
      _tableClosed->erase(obsoleteOrDemotedCI->id);
      //_tableClosed->emplace(obsoleteOrDemotedCI->id, (CloStreamCI*)0);
      delete obsoleteOrDemotedCI->itemset;
      delete obsoleteOrDemotedCI->positions_in_lists;
      delete obsoleteOrDemotedCI;
    }
    else {
      if (obsoleteOrDemotedCI->support == 0) {
        exit(1);
      }
      // we have to increase its support
      const uint32_t oldsupp = obsoleteOrDemotedCI->support;
      obsoleteOrDemotedCI->support -= 1;
      if (obsoleteOrDemotedCI->support > oldsupp) {
        exit(1);
      }
    }
  }
};