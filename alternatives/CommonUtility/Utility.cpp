#include "Utility.h"
#include <iostream>

bool contains(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b, const bool _strict) {
  if ((!_strict && _a->size() == _b->size()) || _a->size() > _b->size()) {
    int lastMatch = 0;
    for (uint32_t i = 0; i != _b->size(); ++i) {
      bool found = false;
      for (uint32_t j = lastMatch; j != _a->size(); ++j) {
        //std::cout << "comparing " << _a->at(j) << " and " << _b->at(i) << std::endl;
        if (_a->at(j) == _b->at(i)) {
          lastMatch = j;
          found = true;
          break;
        }
      }
      if (!found) {
        return false;
      }
    }
    return true;
  }
  return false;
};

std::vector<uint32_t>* inter(std::vector<uint32_t>* const _left_set, std::vector<uint32_t>* const _right_set){
  std::vector<uint32_t>* smaller;
  std::vector<uint32_t>* larger;
  if (_right_set->size() < _left_set->size()) {
    smaller = _right_set; larger = _left_set;
  }
  else {
    smaller = _left_set; larger = _right_set;
  }
  std::vector<uint32_t>* const inter = new std::vector<uint32_t>();
  inter->resize(smaller->size());
  uint32_t curr = 0;

  std::vector<uint32_t>::iterator it_smaller = smaller->begin();
  
  for (; it_smaller != smaller->end(); ++it_smaller) {
    std::vector<uint32_t>::iterator it_larger = larger->begin();
    for (; it_larger != larger->end(); ++it_larger) {
      if (*it_larger == *it_smaller) {
        inter->at(curr) = *it_smaller;
        curr += 1;
        break;
      }
    }
  }

  if (curr != inter->size()) {
    inter->resize(curr);
  }
  return inter;
};

bool exactMatch(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b) {
  if (_a->size() == _b->size()) {
    for (uint32_t i = 0; i != _b->size(); ++i) {
      if (_a->at(i) != _b->at(i)) {
        return false;
      }
    }
    return true;
  }
  return false;
};

void print_array(std::vector<uint32_t>* const _array) {
  std::cout << "{";
  std::vector<uint32_t>::iterator it = _array->begin();
  for (; it != _array->end(); ++it) {
    std::cout << *it << ", ";
  }
  std::cout << "}" << std::endl;
};


std::vector<uint32_t>* diff(std::vector<uint32_t>* const _left_set, std::vector<uint32_t>* const _right_set) {
  std::vector<uint32_t>* smaller;
  std::vector<uint32_t>* larger;
  if (_right_set->size() < _left_set->size()) {
    smaller = _right_set; larger = _left_set;
  }
  else {
    smaller = _left_set; larger = _right_set;
  }
  std::vector<uint32_t>* const diff = new std::vector<uint32_t>();
  diff->resize(larger->size());
  uint32_t curr = 0;

  
  std::vector<uint32_t>::iterator it_larger = larger->begin();

  for (; it_larger != larger->end(); ++it_larger) {
    std::vector<uint32_t>::iterator it_smaller = smaller->begin();
    bool was_found = false;
    for (; it_smaller != smaller->end(); ++it_smaller) {
      if (*it_larger == *it_smaller) {
        was_found = true;
        break;
      }
    }
    if (!was_found) {
      diff->at(curr) = *it_larger;
      curr += 1;
    }
  }
  if (curr != diff->size()) {
    diff->resize(curr);
  }
  //std::cout << "diff is " << diff->size() << std::endl;
  return diff;
};
