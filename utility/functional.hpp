#pragma once


#include <functional>
#include <utility>

#include "numeric/arithmetic.hpp"


namespace lib {

namespace internal {


template<class T> constexpr T plus(const T a, const T b) { return std::plus<T>{}(a, b); }
template<class T> constexpr T minus(const T a, const T b) { return std::minus<T>{}(a, b); }

template<class T> constexpr T bitxor(const T a, const T b) { return a xor b; }


} // namespace internal


template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T1, class T2> inline bool chmin(T1 &a, const T2& b) { return (a>b ? a=b, true : false); }
template<class T1, class T2> inline bool chmax(T1 &a, const T2& b) { return (a<b ? a=b, true : false); }

template<class T1, class... Args> inline bool chmin(T1 &a, Args... b) { return chmin(a, min(b...)); }
template<class T1, class... Args> inline bool chmax(T1 &a, Args... b) { return chmax(a, max(b...)); }


} // namespace lib
