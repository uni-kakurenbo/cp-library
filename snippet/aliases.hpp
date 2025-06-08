#pragma once


#include <cstdint>
#include <utility>
#include <vector>
#include <ranges>


#include "internal/dev_env.hpp"
#include "snippet/internal/types.hpp"
#include "adaptor/string.hpp"


#define until(...) while(!(__VA_ARGS__))

#define CONTINUE(...) { __VA_ARGS__; continue; }
#define BREAK(...) { __VA_ARGS__; break; }

#define ALL(x) std::ranges::begin((x)),std::ranges::end((x))
#define RALL(x) std::ranges::rbegin((x)),std::ranges::rend((x))


#define $F first
#define $S second


namespace uni {


constexpr char LN = '\n';
constexpr char SPC = ' ';


constexpr string DIRECTIONS = "URDL";

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
