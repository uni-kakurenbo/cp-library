#pragma once

#include <functional>

#include <utility>

namespace lib {

namespace internal {


template<class T> constexpr T plus(const T a, const T b) { return std::plus<T>{}(a, b); }
template<class T> constexpr T minus(const T a, const T b) { return std::minus<T>{}(a, b); }

template<class T> constexpr T bxor(const T a, const T b) { return a xor b; }

} // namespace internal

} // namespace lib
