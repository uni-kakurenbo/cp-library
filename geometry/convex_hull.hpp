#pragma once


#include <vector>
#include <iterator>
#include <ranges>


#include "internal/dev_env.hpp"

#include "geometry/point.hpp"
#include "geometry/polygon.hpp"



namespace uni {


template<class Point, class Container>
template<bool ALLOW_LINE, bool LEAVE_MARGIN>
auto polygon<Point, Container>::convex_hull() noexcept(NO_EXCEPT) {
    auto remove = [&](const point_type& p, const point_type& q, const point_type& r) -> bool {
        if constexpr(ALLOW_LINE) {
            return compare(cross(p, q, r)) < 0;
        }
        else {
            return compare(cross(p, q, r)) <= 0;
        }
    };

    std::vector<point_type> points(ALL(*this));

    const auto n = std::ranges::ssize(points);
    using ssize_type = std::remove_const_t<decltype(n)>;

    std::ranges::sort(points);

    polygon res(2 * n);
    ssize_type k = 0;

    for(ssize_type i = 0; i < n; res[k++] = points[i++]) {
        while(k >= 2 && remove(points[i], res[k - 2], res[k - 1])) --k;
    }
    for(auto i = n - 2, t = k + 1; i >= 0; res[k++] = points[i--]) {
        while(k >= t && remove(points[i], res[k - 2], res[k - 1])) --k;
    }

    if constexpr(LEAVE_MARGIN) res.resize(k);
    else res.resize(k - 1);

    return res;
}


} // namespace uni
