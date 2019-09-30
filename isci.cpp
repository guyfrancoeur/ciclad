//isci.cpp utils
//
#include <iostream>
#include <fstream>
#include "isci.h"

template <class T>
void wf(T p, string s) {
  ofstream f;
  f.open(s);
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

extern void freeNode(node *n) {
  auto _enfant = n->enfant;
  auto it1 = _enfant.begin();
  while (it1 != _enfant.end()) {
    if (_enfant.size() > 0) {
      freeNode(it1->second);
    }
    ++it1;
  }
  n->enfant.clear();
  delete(n);
  return;
}
