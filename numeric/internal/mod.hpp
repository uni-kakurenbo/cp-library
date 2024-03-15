#pragma once


#include "internal/concepts.hpp"


namespace lib {



template<class T, class R>
    requires
        internal::weakly_remainder_assignable<T, R> &&
        internal::weakly_addition_assignable<T, R>
inline T mod(T x, const R& r) noexcept(NO_EXCEPT) {
    x %= r;
    if(x < 0) x += r;
    return x;
}


} // namespace lib
