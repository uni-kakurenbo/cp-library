#pragma once


#include <algorithm>
#include <iterator>
#include <concepts>
#include <ranges>

#include "internal/dev_env.hpp"

#include "adapter/valarray.hpp"


namespace lib {


template<std::ranges::input_range R, class F>
inline R applied(R v, F func) noexcept(NO_EXCEPT) {
    func(std::ranges::begin(v), std::ranges::end(v));
    return v;
}

template<std::ranges::input_range R>
inline auto sorted(R&& v) noexcept(NO_EXCEPT) {
    return applied(v, std::ranges::sort);
}

template<std::ranges::input_range R>
inline auto reversed(R v) noexcept(NO_EXCEPT) {
    return applied(v, std::ranges::reverse);
}


} // namespace lib
