#pragma once

#include <functional>

#include <utility>

namespace lib {

namespace internal {


template<class T> constexpr T plus(const T a, const T b) { return std::plus<T>{}(a, b); }
template<class T> constexpr T minus(const T a, const T b) { return std::minus<T>{}(a, b); }

template<class T> constexpr T bitxor(const T a, const T b) { return a xor b; }


} // namespace internal


template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T1, class T2> inline bool chmax(T1 &a, T2 b) { return (a<b ? a=b, true : false); }
template<class T1, class T2> inline bool chmin(T1 &a, T2 b) { return (a>b ? a=b, true : false); }


template<class T> inline constexpr T sign(const T x) {
    return (x > 0) - (x < 0);
}

template<class T, T FROM_MIN, T FROM_MAX, T TO_MIN, T TO_MAX> inline constexpr T mapping(const T x) {
    return (x - FROM_MIN) * (TO_MAX - TO_MIN) / (FROM_MAX - FROM_MIN) + TO_MIN;
}
template<class T> inline constexpr T mapping(const T x, const T from_min, const T from_max, const T to_min, const T to_max) {
    return (x - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}


} // namespace lib
