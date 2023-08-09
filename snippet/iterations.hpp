#pragma once

#include <type_traits>
#include "snippet/internal/overload.hpp"

#define LOOP(n) REPI($_, (n))

#define REPI(i,n) for(std::decay_t<decltype(n)> i=0, i##_length=(n); i<i##_length; ++i)
#define REPF(i,l,r) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>> i=(l), i##_last=(r); i<i##_last; ++i)
#define REPIS(i,l,r,s) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>,std::decay_t<decltype(s)>> i=(l), i##_last=(r); i<i##_last; i+=(s))

#define REPR(i,n) for(auto i=(n); --i>=0;)
#define REPB(i,l,r) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>> i=(r), i##_last=(l); --i>=i##_last;)
#define REPRS(i,l,r,s) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>,std::decay_t<decltype(s)>> i=(l)+((r)-(l)-1)/(s)*(s), i##_last=(l); i>=i##_last; (i-=(s)))

#define REP(...) $OVERLOAD4(__VA_ARGS__, REPIS, REPF, REPI, LOOP)(__VA_ARGS__)
#define REPD(...) $OVERLOAD4(__VA_ARGS__, REPRS, REPB, REPR)(__VA_ARGS__)

#define FORO(i,n) for(int i=0, i##_last=static_cast<int>(n); i<=i##_last; ++i)
#define FORI(i,l,r) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>> i=(l), i##_last=(r); i<=i##_last; ++i)
#define FORIS(i,l,r,s) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>,std::decay_t<decltype(s)>> i=(l), i##_last=(r); i<=i##_last; i+=(s))

#define FORRO(i,n) for(auto i=(n); i>=0; --i)
#define FORR(i,l,r) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>> i=(r), i##_last=(l); i>=i##_last; --i)
#define FORRS(i,l,r,s) for(std::common_type_t<std::decay_t<decltype(l)>,std::decay_t<decltype(r)>,std::decay_t<decltype(s)>> i=(l)+((r)-(l))/(s)*(s), i##_last=(l); i>=i##_last; i-=(s))

#define FOR(...) $OVERLOAD4(__VA_ARGS__, FORIS, FORI, FORO)(__VA_ARGS__)
#define FORD(...) $OVERLOAD4(__VA_ARGS__, FORRS, FORR, FORRO)(__VA_ARGS__)

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
