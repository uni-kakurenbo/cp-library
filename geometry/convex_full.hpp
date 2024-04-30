#pragma once


#include <vector>
#include <iterator>
#include <ranges>

#include "internal/dev_env.hpp"

#include "adaptor/vector.hpp"

#include "geometry/basic.hpp"



namespace uni {


template<bool LEAVE_MARGIN, bool ALLOW_LINE, std::input_iterator I, std::sentinel_for<I> S>
auto convex_hull(I first, S last) noexcept(NO_EXCEPT) {
    using point_type = std::iter_value_t<I>;
    using size_type = internal::size_t;

    auto remove = [&](const point_type& p, const point_type& q, const point_type& r) -> bool {
        if constexpr(ALLOW_LINE) {
            return compare(cross(p, q, r)) < 0;
        }
        else {
            return compare(cross(p, q, r)) <= 0;
        }
    };

    std::vector<point_type> points(first, last);

    const auto n = std::ranges::ssize(points);
    std::ranges::sort(points);

    vector<point_type> res(2 * n);
    size_type k = 0;

    for(size_type i=0; i<n; res[k++] = points[i++]) {
        while(k >= 2 and remove(points[i], res[k-2], res[k-1])) --k;
    }
    for(auto i=n-2, t=k+1; i>=0; res[k++] = points[i--]) {
        while(k >= t and remove(points[i], res[k-2], res[k-1])) --k;
    }

    if constexpr(LEAVE_MARGIN) res.resize(k);
    else res.resize(k - 1);

    return res;
}

template<std::input_iterator I, std::sentinel_for<I> S>
inline auto convex_hull(I first, S last) noexcept(NO_EXCEPT) { return convex_hull<false, true>(first, last); }


template<bool LEAVE_MARGIN, bool ALLOW_LINE, std::ranges::input_range R>
inline auto convex_hull(R&& range) noexcept(NO_EXCEPT) { return convex_hull<LEAVE_MARGIN, ALLOW_LINE>(std::begin(range), std::end(range)); }

template<std::ranges::input_range R>
inline auto convex_hull(R&& range) noexcept(NO_EXCEPT) { return convex_hull<false, true>(std::begin(range), std::end(range)); }


} // namespace uni
