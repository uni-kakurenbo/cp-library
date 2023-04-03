#pragma once

#include <vector>

#include "geometry/basic.hpp"

namespace lib {


template<class T>
std::vector<point<T>> convex_hull(std::vector<point<T>> points) {
    using size_t = internal::size_t;

    size_t n = points.size();
    std::sort(points.begin(), points.end());

    std::vector<point<T>> res;

    for(const auto p : points) {
        while(res.size() >= 2UL and cross(res.end()[-1], res.end()[-2], p) >= 0) {
            res.pop_back();
        }
        res.push_back(p);
    }

    size_t t = res.size();
    for(auto i=n-2; i>=0; --i) {
        const auto p = points[i];
        while(static_cast<size_t>(res.size()) > t and cross(res.end()[-1], res.end()[-2], p) >= 0) {
            res.pop_back();
        }
        res.push_back(p);
    }

    return res;
}


} // namespace lib
