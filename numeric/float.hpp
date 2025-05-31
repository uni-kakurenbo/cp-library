#pragma once


#include <cstdint>
#include <limits>


#include "internal/exception.hpp"
#include "internal/concepts.hpp"

#include "numeric/limits.hpp"
#include "numeric/arithmetic.hpp"


namespace uni {


namespace internal {


template<class T>
inline std::int32_t compare(const T x, const T y, const T tolerance) noexcept(NO_EXCEPT) {
    if(x > y + tolerance) return 1;
    if(y > x + tolerance) return -1;
    return 0;
}


template<class T>
inline std::int32_t compare(const T x, const T y = 0) noexcept(NO_EXCEPT) {
    if constexpr(std::is_floating_point_v<T>) {
        return compare(x, y, max(1, x, y) * numeric_limits<T>::arithmetic_epsilon());
    }
    else {
        if(x > y) return 1;
        if(x < y) return -1;
        return 0;
    }
}


} // namespace internal

template<class T0, std::common_with<T0> T1>
    requires (
        !(std::is_floating_point_v<T0> && std::is_floating_point_v<T1>) ||
        std::is_floating_point_v<std::common_type_t<T0, T1>>
    )
inline std::int32_t compare(const T0 v0, const T1 v1) noexcept(NO_EXCEPT) {
    return internal::compare<std::common_type_t<T0, T1>>(v0, v1);
}


} // namespace uni
