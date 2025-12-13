#pragma once


#include <cstdint>
#include <utility>
#include <vector>
#include <queue>
#include <ranges>

#include "macro/internal/compatibility.hpp"

#include "internal/dev_env.hpp"

#include "snippet/internal/base.hpp"
#include "snippet/internal/types.hpp"

#include "adaptor/string.hpp"


namespace uni {


constexpr char LN = '\n';
constexpr char SPC = ' ';


UNI_CONSTEXPR_STRING DIRECTIONS = "URDL";

constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS4P[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }, { 0, 0 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };
constexpr std::pair<int,int> DIRS8P[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { 0, 0 } };


template<class T>
using spair = std::pair<T,T>;


template<class T>
using priority_queue_rev = std::priority_queue<T, std::vector<T>, std::greater<T>>;


}  // namespace uni


namespace std {

using bit_reference = std::vector<bool>::reference;

bit_reference operator |= (bit_reference a, const bool b) noexcept(NO_EXCEPT) { return a = a | b; }
bit_reference operator &= (bit_reference a, const bool b) noexcept(NO_EXCEPT) { return a = a & b; }

}
