#pragma once

#include <iostream>

#ifdef LOCAL_JUDGE
    #include<debug>
    #define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
    #define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);
#else
    #define debug(...) ({ ; })
    #define DEBUG(...) ({ ; })
#endif

#define INF32 2147483647
#define INF64 9223372036854775807LL

#define until(...) while(!(__VA_ARGS__))

#include "internal/macro.hpp"

#define LOOP(n) REPI($_, (n))

#define REPI(i,n) for(int i=0, i##_length=int(n); i<i##_length; ++i)
#define REPF(i,l,r) for(auto i=(l), i##_last=(r); i!=i##_last; ++i)
#define REPIS(i,l,r,s) for(auto i=(l), i##_last=(r); i!=i##_last; i+=(s))

#define REPR(i,n) for(auto i=(n); --i>=0;)
#define REPB(i,l,r) for(auto i=(r), i##_last=(l); --i>=i##_last;)
#define REPRS(i,l,r,s) for(auto i=(r), i##_last=(l); (i-=(s))>=i##_last;)

#define REP(...) $OVERLOAD4(__VA_ARGS__, REPIS, REPF, REPI, LOOP)(__VA_ARGS__)
#define REPD(...) $OVERLOAD4(__VA_ARGS__, REPRS, REPF, REPB)(__VA_ARGS__)

#define FORI(i,l,r) for(auto i=(l), i##_last=(r); i<=i##_last; ++i)
#define FORIS(i,l,r,s) for(auto i=(l), i##_last=(r); i<=i##_last; i+=(s))
#define FORR(i,l,r) for(auto i=(l), i##_last=(r); i>=i##_last; --i)
#define FORRS(i,l,r,s) for(auto i=(l), i##_last=(r); i>=i##_last; i-=(s))

#define FOR(...) $OVERLOAD4(__VA_ARGS__, FORIS, FORI)(__VA_ARGS__)
#define FORD(...) $OVERLOAD4(__VA_ARGS__, FORDS, FORD)(__VA_ARGS__)

#define ITR1(e0,v) for(const auto &e0 : (v))
#define ITRP1(e0,v) for(auto e0 : (v))
#define ITRR1(e0,v) for(auto &e0 : (v))

#define ITR2(e0,e1,v) for(const auto [e0, e1] : (v))
#define ITRP2(e0,e1,v) for(auto [e0, e1] : (v))
#define ITRR2(e0,e1,v) for(auto &[e0, e1] : (v))

#define ITR3(e0,e1,e2,v) for(const auto [e0, e1, e2] : (v))
#define ITRP3(e0,e1,e2,v) for(auto [e0, e1, e2] : (v))
#define ITRR3(e0,e1,e2,v) for(auto &[e0, e1, e2] : (v))

#define ITR4(e0,e1,e2,e3,v) for(const auto [e0, e1, e2, e3] : (v))
#define ITRP4(e0,e1,e2,e3,v) for(auto [e0, e1, e2, e3] : (v))
#define ITRR4(e0,e1,e2,e3,v) for(auto &[e0, e1, e2, e3] : (v))

#define ITR(...) $OVERLOAD5(__VA_ARGS__, ITR4, ITR3, ITR2, ITR1)(__VA_ARGS__)
#define ITRP(...) $OVERLOAD5(__VA_ARGS__, ITRP4, ITRP3, ITRP2, ITRP1)(__VA_ARGS__)
#define ITRR(...) $OVERLOAD5(__VA_ARGS__, ITRR4, ITRR3, ITRR2, ITRR1)(__VA_ARGS__)

#define ALL(x) std::begin((x)),std::end((x))
#define RALL(x) std::rbegin((x)),std::rend((x))

#define $F first
#define $S second

using ll = long long;
using ull = unsigned long long;
using ld = long double;

constexpr char ln = '\n';
constexpr char spc = ' ';

#include <utility>

constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };

__attribute__((constructor)) inline void fast_io() { std::ios::sync_with_stdio(false), std::cin.tie(nullptr); }

template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T1, class T2> inline bool chmax(T1 &a, T2 b) { return (a<b ? a=b, true : false); }
template<class T1, class T2> inline bool chmin(T1 &a, T2 b) { return (a>b ? a=b, true : false); }

#include "input.hpp"
#include "output.hpp"

Input _input;
Output _print;

#define input _input
#define print _print
