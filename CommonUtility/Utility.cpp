#include "Utility.h"

bool contains(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b, const bool _strict) {
  if ((!_strict && _a->size() == _b->size()) || _a->size() > _b->size()) {
    int lastMatch = 0;
    for (uint32_t i = 0; i != _b->size(); ++i) {
      bool found = false;
      for (int j = lastMatch; j != _a->size(); ++j) {
        if (_a[j] == _b[i]) {
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
  std::vector<uint32_t> inter = std::vector<uint32_t>();
  inter.resize(smaller->size());
  int curr = 0;

  std::vector<uint32_t>::iterator it_smaller = smaller->begin();
  
  for (; it_smaller != smaller->end(); ++it_smaller) {
    std::vector<uint32_t>::iterator it_larger = larger->begin();
    for (; it_larger != larger->end(); ++it_larger) {
      if (*it_larger == *it_smaller) {
        inter[curr] = *it_smaller;
        curr += 1;
        break;
      }
    }
  }

  if (curr != inter.size()) {
    inter.resize(curr);
  }
  return &inter;
};

bool exactMatch(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b) {
  if (_a->size() == _b->size()) {
    for (int i = 0; i != _b->size(); ++i) {
      if (_a[i] != _b[i]) {
        return false;
      }
    }
    return true;
  }
  return false;
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
  std::vector<uint32_t> diff = std::vector<uint32_t>();
  diff.resize(larger->size());
  int curr = 0;

  std::vector<uint32_t>::iterator it_smaller = smaller->begin();

  for (; it_smaller != smaller->end(); ++it_smaller) {
    std::vector<uint32_t>::iterator it_larger = larger->begin();
    bool was_found = false;
    for (; it_larger != larger->end(); ++it_larger) {
      if (*it_larger == *it_smaller) {
        was_found = true;
        break;
      }
    }
    if (!was_found) {
      diff[curr] = *it_larger;
      curr += 1;
    }
  }
  if (curr != diff.size()) {
    diff.resize(curr);
  }
  return &diff;
};
