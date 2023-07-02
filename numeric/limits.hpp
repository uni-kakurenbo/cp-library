#pragma once


#include <limits>

#include "internal/dev_env.hpp"


namespace lib {


template<class T>
struct numeric_limits : std::numeric_limits<T> {
    static constexpr T arithmetic_infinity() noexcept(NO_EXCEPT) {
        return std::numeric_limits<T>::max() / 2 - 1;
    }

    static constexpr T arithmetic_negative_infinity() noexcept(NO_EXCEPT) {
        return std::numeric_limits<T>::lowest() / 2 + 1;
    }
};


} // namespace lib
