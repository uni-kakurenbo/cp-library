#pragma once

#include <vector>
#include <iterator>

#include "internal/dev_env.hpp"

#include "geometry/basic.hpp"


namespace lib {


template<class I, class P = typename std::iterator_traits<I>::value_type>
std::vector<P> convex_hull(const I first, const I last, const bool margin = false) noexcept(NO_EXCEPT) {
    using size_t = internal::size_t;

    std::vector<P> points(first, last);

    const auto n = static_cast<size_t>(points.size());
    std::sort(points.begin(), points.end());

    std::vector<P> res;

    for(const auto& p : points) {
        while(res.size() >= 2UL and cross(p, res.end()[-2], res.end()[-1]) <= 0) {
            res.pop_back();
        }
        res.emplace_back(p);
    }

    const auto t = static_cast<size_t>(res.size());
    for(auto i=n-2; i>=0; --i) {
        const auto p = points[i];
        while(static_cast<size_t>(res.size()) > t and cross(p, res.end()[-2], res.end()[-1]) <= 0) {
            res.pop_back();
        }
        if(not margin and i == 0) break;
        res.emplace_back(p);
    }

    return res;
}


} // namespace lib
