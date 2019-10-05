#ifndef UTILITY_ALG_H
#define UTILITY_ALG_H

#include <vector>

bool contains(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b, const bool _strict);

std::vector<uint32_t>* inter(std::vector<uint32_t>* const left_set, std::vector<uint32_t>* const right_set);

bool exactMatch(std::vector<uint32_t>* const _a, std::vector<uint32_t>* const _b);

std::vector<uint32_t>* diff(std::vector<uint32_t>* const left_set, std::vector<uint32_t>* const right_set);

#endif // !UTILITY_ALG_H

