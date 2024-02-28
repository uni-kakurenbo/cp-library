#pragma once


#include <concepts>
#include <utility>


#include "algebraic/base.hpp"
#include "algebraic/internal/concepts.hpp"


namespace lib {

namespace algebraic {

namespace internal {

struct dummy {};

} // namespace internal


template<
    internal::magma M0,
    internal::magma M1
>
struct combined
  : base<std::pair<M0, M1>>,
    std::conditional_t<internal::associative<M0> && internal::associative<M1>, associative, internal::dummy>,
    std::conditional_t<internal::commutative<M0> && internal::commutative<M1>, commutative, internal::dummy>
{
    using base<std::pair<M0, M1>>::base;

    combined(const M0& v0, const M1& v1) : base<std::pair<M0, M1>>({ v0, v1 }) {};

    template<class T>
        requires std::convertible_to<T, M0> && std::convertible_to<T, M1>
    combined(const T& v) : combined(v, v) {};

    friend inline combined operator+(const combined& lhs, const combined& rhs) noexcept(NO_EXCEPT) {
        return { lhs->first + rhs->first, lhs->second + rhs->second };
    }

    friend inline combined operator-(const combined& val) noexcept(NO_EXCEPT)
        requires internal::invertible<M0> && internal::invertible<M1>
    {
        return { -val->first, -val->second };
    }
};


} // namespace algebraic

} // namespace lib
