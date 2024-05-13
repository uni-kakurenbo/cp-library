#pragma once


#include <utility>
#include <limits>
#include <functional>

#include "internal/dev_env.hpp"
#include "algebraic/base.hpp"


namespace uni {

namespace algebraic {


template<class T>
struct minmax : base<std::pair<T, T>>, scalar_multipliable<minmax<T>>::identity, associative, commutative {
    using base<std::pair<T, T>>::base;

    minmax() noexcept(NO_EXCEPT)
      : minmax(std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest())
    {};

    minmax(const T& v) noexcept(NO_EXCEPT) : minmax(v, v) {};


    friend inline minmax operator+(const minmax& lhs, const minmax& rhs) noexcept(NO_EXCEPT) {
        return minmax({ std::min(lhs.val().first, rhs->first), std::max(lhs.val().second, rhs->second) });
    }
};


} // namespace algebraic

} // namespace uni
