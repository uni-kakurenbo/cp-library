#pragma once


#include "internal/concepts.hpp"


namespace uni {



template<class T, class R>
    requires
        internal::remainder_calculable<T, R> &&
        internal::subtractable<T, R> &&
        internal::unary_subtractable<T>
inline T mod(T x, const R& r) noexcept(NO_EXCEPT) {
    if(x >= 0) return x % r;
    x = -x % r;
    if(x != 0) x = r - x;
    return x;
}


} // namespace uni
