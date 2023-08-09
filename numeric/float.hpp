#pragma once


#include <cstdint>
#include <limits>

#include "numeric/limits.hpp"
#include "numeric/arithmetic.hpp"


namespace lib {


template<class T>
inline std::int32_t compare(const T x, const T y, const T tolerance) {
    if(x > y + tolerance) return 1;
    if(y > x + tolerance) return -1;
    return 0;
}


template<class T>
inline std::int32_t compare(const T x, const T y = 0) {
    return compare(x, y, max(1, x, y) * numeric_limits<T>::arithmetic_epsilon());
}


} // namespace lib
