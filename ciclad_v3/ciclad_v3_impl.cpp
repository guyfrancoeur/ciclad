#include "ciclad_v3_impl.h"

#pragma warning(disable : 4996)

double add(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid) {
  clock_t start = clock(); clock_t running = clock();

  node3 *root = new node3(_rootChild);
  // à reviser --->
  uint allocated_memory = 0; uint allocated_block = 0;
  uint requested_memory = (uint)fCI2.size() * sizeof(node3 *);
  node3 **li = (node3 **)malloc(requested_memory); //calloc ??
  for (uint i = 0; i < allocated_block; ++i) {
    if (li + i != NULL)
      *(li + i) = NULL;
  }
  memset(li + allocated_block, 0, requested_memory - allocated_memory);
  allocated_memory = requested_memory;
  allocated_block = (uint)fCI2.size();
  // <---
  char *pch = strtok(s, " ");
  // il faut valide la trx dans s
  while (pch != 0) {
    uint item = atol(pch);
    //---- mise a jour du superconcept (bottom) et de son index ---- start
    if (idx[item].size() == 0) {
      vector<uint> v(1, 0); //reserve one space with value 0.
      idx[item] = v;
      if (fCI2[0].supp > 0) {
        concept3 c(*gCid, fCI2[0].supp, fCI2[0].size, 0); //concept2 c(gCid, fCI2[0].supp, fCI2[0].size, fCI2[0].itemset);
        for (uint t = 0; t < fCI2[0].itemset.size(); ++t) {
          idx[fCI2[0].itemset[t]].push_back(*gCid);
        }
        fCI2.push_back(c); ++(*gCid);
        fCI2[0].supp = 0;
      }
      fCI2[0].itemset.push_back(item);
      fCI2[0].size = fCI2[0].size + 1;
    }
    //---- mise a jour du superconcept (bottom) et de son index ---- fin
    for (uint idConcept : idx[item]) { //tant qu'il y a des concepts (pour un item).
      concept3 *e = &fCI2[idConcept]; //fCI2.at(idConcept); //e est un concept 
      //if (e->deleted == 1) {
      //  idx[item].at(i) = idx[item].back();
      //  idx[item].pop_back();
      //  continue;
      //}
      node3 *lin; //lin =est= last item node
      if (li[idConcept] != 0) { //v2.1g rev2
        lin = li[idConcept]; //v2.1g rev2
      }
      else {
        lin = root; //root node
      }
      //------------------------------------------------------- DEBUT ---- UPDATE INTERSECTION
      node3 *n;
      auto *_child = &lin->enfant;  //un petit truc pour les différentes declarations.  :)
      auto it2 = _child->find(item);
      if (it2 == _child->end()) {
        tlx::btree_map<uint, node3 *> _enfant;
        n = new node3(_enfant);
        n->key = item;
        n->Cid = e->id;
        n->parent = lin;
        n->depth = lin->depth + 1;
        n->nb_ref = 0;
        lin->enfant.insert2(lin->enfant.end(), item, n);
        tn.push(n);
      }
      else {
        n = it2->second;
        concept3 *min = &fCI2.at(n->Cid);
        if (min->supp < e->supp) {
          n->Cid = e->id;
        }
      }
      ++n->nb_ref;
      if (lin->nb_ref > 0)
        --lin->nb_ref;
      li[e->id] = n;
      //------------------------------------------------------ FIN ---- UPDATE INTERSECTION
    }
    pch = strtok(0, " ");
  }

  while (!tn.empty()) {
    node3 *n = tn.front();
    if (n->nb_ref > 0) {
      uint newSupp = 1 + fCI2[n->Cid].supp;

      if (fCI2[n->Cid].size == n->depth) {
        fCI2[n->Cid].supp = newSupp;
      }
      else {
        ushort u = 0;
        while (n->parent != NULL) {
          ++u; //size of itemset
          idx[n->key].push_back(*gCid); //index update
          n = n->parent;
        }
        concept3 C3(*gCid, newSupp, u, 0);
        fCI2.push_back(C3); ++(*gCid);
      }
    }
    tn.pop();
  }
  free(li);
  delete root;
  return (clock() - start) / (double)CLOCKS_PER_SEC;
}

double del(char *s, std::queue<node3 *> &tn, std::vector<vector<uint>> &idx, tlx::btree_map<uint, node3 *> &_rootChild, std::vector<concept3> &fCI2, uint *gCid) {
  clock_t start = clock(); clock_t running = clock();

  node3 *root = new node3(_rootChild);
  //ici nous avons besoin d'une optimisation sans arguments --->
  uint allocated_memory = 0; uint allocated_block = 0;
  uint requested_memory = (uint)fCI2.size() * sizeof(node3 *);
  node3 **li = (node3 **)malloc(requested_memory);
  for (uint i = 0; i < allocated_block; ++i) {
    if (li + i != NULL)
      *(li + i) = NULL;
  }
  memset(li + allocated_block, 0, requested_memory - allocated_memory);
  allocated_memory = requested_memory;
  allocated_block = (uint)fCI2.size();
  // <---
  char *pch = strtok(s, " ");
  while (pch != 0) {
    uint item = atol(pch);
    for (uint i = 0; i < idx[item].size(); ++i) { //tant qu'il y a des concepts (pour un item).
      uint idConcept = idx[item].at(i);
      concept3 *e = &fCI2.at(idConcept); //auto e = fCI2.at(idConcept); //e est un concept fCI2[idConcept]
      if (e->deleted == 1) {
        idx[item].at(i) = idx[item].back();
        idx[item].pop_back();
        continue;
      }
      node3 *lin; //lin =est= last item node
      if (li[idConcept] != 0) { //v2.1g rev2
        lin = li[idConcept];    //v2.1g rev2
      }
      else {
        lin = root; //root node
      }
      //------------------------------------------------------- DEBUT ---- UPDATE INTERSECTION
      node3 *n;
      auto *_child = &lin->enfant;  //un petit truc pour les différentes declarations.  :)
      auto it2 = _child->find(item);
      if (it2 == _child->end()) {
        tlx::btree_map<uint, node3 *> _enfant;
        n = new node3(_enfant);
        n->key = item;
        n->Cid = e->id;
        n->parent = lin;
        n->depth = lin->depth + 1;
        n->nb_ref = 0;
        lin->enfant.insert2(lin->enfant.end(), item, n);
        tn.push(n);
      }
      else {
        n = it2->second;
        concept3 *min = &fCI2.at(n->Cid);
        signed int ecart = e->supp - min->supp;
        switch (ecart) {
          case 0: n->Cid = e->id; break;
          case 1:  n->gen.clear(); n->gen.push_back(n->Cid); n->Cid = e->id; break;
          case -1: n->gen.push_back(e->id); break; //tomas
          default:
            if (ecart > 1) {
              n->gen.clear();
              n->Cid = e->id;
            }
            break;
        }
      }
      ++n->nb_ref;
      if (lin->nb_ref > 0)
        --lin->nb_ref;
      li[e->id] = n;
      //------------------------------------------------------ FIN ---- UPDATE INTERSECTION
    }
    pch = strtok(0, " ");
  }

  while (!tn.empty()) {
    node3 *n = tn.front();
    if (n->nb_ref > 0) {
      char state = 1; //bool

      for (auto cid : n->gen) {
        node3 * tmp = li[cid];
        if (tmp == n) {
          state = 0;
          break;
        }
      }
      if (state == 1) {
        fCI2.at(n->Cid).supp--;
      }
      else {
        //cleanup index;
        fCI2.at(n->Cid).deleted = 1;
        auto t = fCI2.at(n->Cid);
        auto it = std::find(fCI2.begin(), fCI2.end(), t);
        if (it != fCI2.end()) fCI2.erase(it);
      }
    }
    tn.pop();
  }
  free(li);
  delete root;
  return (clock() - start) / (double)CLOCKS_PER_SEC;
}

template <class T>
void wf(T p, string s) {
  ofstream f;
  f.open(s, ios_base::out);
  for (unsigned int i = 0; i < p.size(); ++i) {
    f << "id:" << p.at(i).id << " supp:" << p.at(i).supp << " {";
    for (unsigned int j = 0; j < p.at(i).itemset.size(); ++j) {
      f << p.at(i).itemset.at(j) << " ";
    }
    f << "}" << endl;
  }

  f.close();
  return;
}

extern void freeNode3(node3 *n) {
  auto _enfant = n->enfant;
  auto it1 = _enfant.begin();
  while (it1 != _enfant.end()) {
    if (_enfant.size() > 0) {
      freeNode3(it1->second);
    }
    ++it1;
  }
  n->enfant.clear();
  delete(n);
  return;
}