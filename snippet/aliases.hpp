#pragma once

#include "snippet/internal/types.hpp"

#define until(...) while(!(__VA_ARGS__))

#define ALL(x) std::begin((x)),std::end((x))
#define RALL(x) std::rbegin((x)),std::rend((x))

#define $F first
#define $S second

constexpr char ln = '\n';
constexpr char spc = ' ';

#include <utility>
constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };

template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T1, class T2> inline bool chmax(T1 &a, T2 b) { return (a<b ? a=b, true : false); }
template<class T1, class T2> inline bool chmin(T1 &a, T2 b) { return (a>b ? a=b, true : false); }
