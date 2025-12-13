#pragma once


#include <ranges>


#define until(...) while(!(__VA_ARGS__))

#define CONTINUE(...) { __VA_ARGS__; continue; }
#define BREAK(...) { __VA_ARGS__; break; }

#define ALL(x) std::ranges::begin((x)),std::ranges::end((x))
#define RALL(x) std::ranges::rbegin((x)),std::ranges::rend((x))


#define $F first
#define $S second
