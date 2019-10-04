#include ciclad_insert.h

double add(char *s, auto tn, auto idx, node3 _rootchild, concept3 superconcept, auto fCI2, uint gCid) {
  clock_t start = clock(); clock_t running = clock();
  
  node3 *root = new node3(_rootChild);
  uint requested_memory = (uint)fCI2.size()*sizeof(node3 *);
  li = (node3 **)realloc(li, requested_memory);
  for (uint i = 0; i < allocated_block; ++i) {
    if (li + i != NULL)
      *(li + i) = NULL;
  }
  memset(li + allocated_block, 0, requested_memory - allocated_memory);
  allocated_memory = requested_memory;
  allocated_block = (uint)fCI2.size();

  char *pch = strtok(s, " ");
  while (pch != 0) {
    uint item = atol(pch);
    //---- mise a jour du superconcept (bottom) et de son index ---- start
    if (idx[item].size() == 0) {
      vector<uint> v(1, 0); //reserve one space with value 0.
      idx[item] = v;
      if (fCI2[0].supp > 0) {
        concept3 c(gCid, fCI2[0].supp, fCI2[0].size); //concept2 c(gCid, fCI2[0].supp, fCI2[0].size, fCI2[0].itemset);
        for (uint t = 0; t < fCI2[0].itemset.size(); ++t) {
          idx[fCI2[0].itemset[t]].push_back(gCid);
        }
        fCI2.push_back(c); ++gCid;
        fCI2[0].supp = 0;
      }
      fCI2[0].itemset.push_back(item);
      fCI2[0].size = fCI2[0].size + 1;
    }
    //---- mise a jour du superconcept (bottom) et de son index ---- fin
    for (uint idConcept : idx[item]) { //tant qu'il y a des concepts (pour un item).
      concept3 *e = &fCI2.at(idConcept); //auto e = fCI2.at(idConcept); //e est un concept fCI2[idConcept]
      node3 *lin; //lin =est= last item node
      if (li[idConcept] != 0) { //v2.1g rev2
        lin = li[idConcept]; //v2.1g rev2
      } else {
        lin = root; //root node
      }
      //------------------------------------------------------- DEBUT ---- UPDATE INTERSECTION
      node3 *n;
     auto *_child = &lin->enfant;  //un petit truc pour les différentes declarations.  :)
      auto it2 = _child->find(item);
      if (it2 == _child->end()) {
        stx::btree_map<uint, node3 *> _enfant;
        n = new node3(_enfant);
        n->key = item;
        n->Cid = e->id;
        n->parent = lin;
        n->depth = lin->depth + 1;
        n->nb_ref = 0;
        lin->enfant.insert2(lin->enfant.end(), item, n);
        tn.push(n);
      } else {
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
      } else {
        ushort u=0;
        while (n->parent != NULL) {
          ++u; //size of itemset
          idx[n->key].push_back(gCid); //index update
          n = n->parent;
        }
        concept3 C3(gCid, newSupp, u);
        fCI2.push_back(C3); ++gCid;
      }
    }
    tn.pop();
  }
  return (clock() - start) / (double)CLOCKS_PER_SEC;
}
