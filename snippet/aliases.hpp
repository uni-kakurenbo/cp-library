#pragma once


#include <cstdint>
#include <utility>
#include <vector>
#include <ranges>


#include "internal/dev_env.hpp"
#include "snippet/internal/types.hpp"


#define until(...) while(!(__VA_ARGS__))

#define CONTINUE(...) { __VA_ARGS__; continue; }
#define BREAK(...) { __VA_ARGS__; break; }

#define ALL(x) std::ranges::begin((x)),std::ranges::end((x))
#define RALL(x) std::ranges::rbegin((x)),std::ranges::rend((x))


#define $F first
#define $S second


namespace lib {


template<class T> using spair = std::pair<T,T>;


}


namespace std {

using bit_reference = std::vector<bool>::reference;

bit_reference operator |= (bit_reference a, const bool b) noexcept(NO_EXCEPT) { return a = a | b; }
bit_reference operator &= (bit_reference a, const bool b) noexcept(NO_EXCEPT) { return a = a & b; }

}
