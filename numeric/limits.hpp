#pragma once


#include <limits>
#include <type_traits>


#include "snippet/internal/types.hpp"

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


constexpr i32 INF32 = numeric_limits<i32>::arithmetic_infinity();
constexpr i64 INF64 = numeric_limits<i64>::arithmetic_infinity();

template<class T>
constexpr T INF = numeric_limits<T>::arithmetic_infinity();

template<class T>
constexpr T EPSILON = numeric_limits<T>::arithmetic_epsilon();


} // namespace lib
