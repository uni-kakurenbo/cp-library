#pragma once

#include <cstdint>
#include <utility>

#include "snippet/internal/types.hpp"


#define until(...) while(!(__VA_ARGS__))

#define ALL(x) std::begin((x)),std::end((x))
#define RALL(x) std::rbegin((x)),std::rend((x))

#define $F first
#define $S second

namespace lib {


template<class T> using spair = std::pair<T,T>;


}
