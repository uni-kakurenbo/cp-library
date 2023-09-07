#pragma once


#include <cstdint>
#include <utility>
#include <vector>

#if CPP20

#include <ranges>

#endif


#include "internal/dev_env.hpp"
#include "snippet/internal/types.hpp"


#define until(...) while(!(__VA_ARGS__))

#if CPP20

#define ALL(x) std::ranges::begin((x)),std::ranges::end((x))
#define RALL(x) std::ranges::rbegin((x)),std::ranges::rend((x))

#else

#define ALL(x) std::begin((x)),std::end((x))
#define RALL(x) std::rbegin((x)),std::rend((x))

#endif

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
