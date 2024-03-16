#pragma once


#include <ranges>
#include <concepts>


#include "snippet/aliases.hpp"

#include "numeric/divisor_multiple_transform.hpp"

#include "adaptor/valarray.hpp"


namespace lib {


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
Res lcm_convolution(R0 v0, R1 v1) {
    assert(std::ranges::size(v0) == std::ranges::size(v1));

    divisor_transform::zeta(v0), divisor_transform::zeta(v1);
    REP(i, std::ranges::size(v0)) v0[i] *= v1[i];
    divisor_transform::mobius(v0);

    if constexpr(std::same_as<Res, R0>) return v0;
    else return Res(ALL(v0));
}


template<std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto lcm_convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return lcm_convolution<valarray<common_type>>(v0, v1);
}


} // namespace lib
