#pragma once


#include <ranges>
#include <concepts>


#include "snippet/aliases.hpp"

#include "numeric/divisor_multiple_transform.hpp"

#include "adaptor/valarray.hpp"


namespace uni {


template<std::ranges::range Res, std::size_t OFFSET, std::ranges::sized_range R0, std::ranges::sized_range R1>
Res gcd_convolution(R0 v0, R1 v1) {
    assert(std::ranges::size(v0) == std::ranges::size(v1));

    multiple_transform::zeta<OFFSET>(v0), multiple_transform::zeta<OFFSET>(v1);
    REP(i, std::ranges::size(v0)) v0[i] *= v1[i];
    multiple_transform::mobius<OFFSET>(v0);

    if constexpr(std::convertible_to<R0, Res>) return v0;
    else return Res(ALL(v0));
}


template<std::size_t OFFSET, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto gcd_convolution(R0&& v0, R1&& v1) {
    using common_type = std::common_type_t<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>;
    return gcd_convolution<valarray<common_type>, OFFSET>(v0, v1);
}


template<std::ranges::range Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto gcd_convolution(R0&& v0, R1&& v1) { return gcd_convolution<Res, 0>(v0, v1); }


template<std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires std::common_with<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>>
auto gcd_convolution(R0&& v0, R1&& v1) { return gcd_convolution<0>(v0, v1); }


} // namespace uni
