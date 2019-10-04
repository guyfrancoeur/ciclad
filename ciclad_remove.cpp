#include ciclad_remove.h

double del(char *s, auto &tn, auto &idx, auto &_rootchild, auto &fCI2, uint *gCid) {
  clock_t start = clock(); clock_t running = clock();

  node3 *root = new node3(_rootChild);
  //ici nous avons besoin d'une optimisation sans arguments --->
  uint requested_memory = (uint)fCI2.size() * sizeof(node3 *);
  node3 li = (node3 **)realloc(li, requested_memory);
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
    for (uint i=0; i < idx[item].size();++i) { //tant qu'il y a des concepts (pour un item).
      uint idConcept = idx[item].at(i);
      concept3 *e = &fCI2.at(idConcept); //auto e = fCI2.at(idConcept); //e est un concept fCI2[idConcept]
      if (e.deleted == 1) {
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
        stx::btree_map<uint, node3 *> _enfant;
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
           0: n->Cid = e->id; break;
           1:  n->gen.clear(); n->gen.push_back(n->Cid); n->Cid = e->id; break;
          -1: n->gen.push_back(e->id); break; //tomas
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
      } else {
        //cleanup index;
        fCI2.at(n->Cid).deleted = 1;
        fCI2.erase(n->Cid);
      }
    }
    tn.pop();
  }
  return (clock() - start) / (double)CLOCKS_PER_SEC;
}
