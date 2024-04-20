#pragma once


#include <algorithm>
#include <iterator>
#include <concepts>
#include <ranges>

#include "internal/dev_env.hpp"

#include "adaptor/valarray.hpp"


namespace uni {


template<std::ranges::range R, class F>
inline R applied(R v, F&& func) noexcept(NO_EXCEPT) {
    func(std::ranges::begin(v), std::ranges::end(v));
    return v;
}

template<std::ranges::range R>
inline auto sorted(R&& v) noexcept(NO_EXCEPT) {
    return applied(std::forward<R>(v), std::ranges::sort);
}

template<std::ranges::range R>
inline auto reversed(R&& v) noexcept(NO_EXCEPT) {
    return applied(std::forward<R>(v), std::ranges::reverse);
}


} // namespace uni
