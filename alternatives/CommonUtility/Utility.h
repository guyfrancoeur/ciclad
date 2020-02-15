#ifndef UTILITY_ALG_H
#define UTILITY_ALG_H

#include <vector>

class LexicographicItemsetComparator {
public:
  bool compare(std::vector<uint32_t>* const _lhs, std::vector<uint32_t>* const _rhs) const {
    const uint32_t smaller = _lhs->size() > _rhs->size() ? _rhs->size() : _lhs->size();
    for (uint32_t i = 0; i != smaller; ++i) {
      if (_lhs->at(i) > _rhs->at(i)) return false;
      else if (_lhs->at(i) < _rhs->at(i)) return true;
    }
    if (_lhs->size() > _rhs->size()) {
      return false;
    }
    else if (_lhs->size() < _rhs->size()) {
      return true;
    }
    return false;
  };

  bool operator() (std::vector<uint32_t>* const _lhs, std::vector<uint32_t>* const _rhs) const {
    return compare(_lhs, _rhs);
  };
};

bool contains(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b, const bool _strict);

std::vector<uint32_t>* inter(std::vector<uint32_t>* const left_set, std::vector<uint32_t>* const right_set);

bool exactMatch(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b);

std::vector<uint32_t>* diff(std::vector<uint32_t>* const left_set, std::vector<uint32_t>* const right_set);

void print_array(std::vector<uint32_t>* const _array);

#endif // !UTILITY_ALG_H

