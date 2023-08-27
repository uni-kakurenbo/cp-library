#pragma once


#include <limits>
#include <type_traits>


#include "internal/dev_env.hpp"


namespace lib {


template<class T>
struct numeric_limits : std::numeric_limits<T> {
    static constexpr long double FLOAT_EPSILON = 1E-14;

    static constexpr T arithmetic_infinity() noexcept(NO_EXCEPT) {
        return std::numeric_limits<T>::max() / 2 - 1;
    }

    static constexpr T arithmetic_negative_infinity() noexcept(NO_EXCEPT) {
        return std::numeric_limits<T>::lowest() / 2 + 1;
    }

    static constexpr T arithmetic_epsilon() noexcept(NO_EXCEPT) {
        if constexpr(std::is_floating_point_v<T>) {
            return numeric_limits::FLOAT_EPSILON;
        }
        else {
            return 0;
        }
    }
};


} // namespace lib
