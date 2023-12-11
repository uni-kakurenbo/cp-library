#pragma once


#include <ranges>
#include <concepts>


#include "snippet/aliases.hpp"

#include "numeric/divisor_multiple_transform.hpp"

#include "adapter/valarray.hpp"


namespace lib {


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
Res gcd_convolution(R0 v0, R1 v1) {
    assert(std::ranges::size(v0) == std::ranges::size(v1));

    multiple_transform::zeta(v0), multiple_transform::zeta(v1);
    REP(i, std::ranges::size(v0)) v0[i] *= v1[i];
    multiple_transform::mobius(v0);

    if constexpr(std::same_as<Res, R0>) return v0;
    else return Res(ALL(v0));
}


template<std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto gcd_convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return gcd_convolution<valarray<common_type>>(v0, v1);
}


} // namespace lib
