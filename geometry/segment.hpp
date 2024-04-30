#pragma once


#include <tuple>

#include "utility/functional.hpp"

#include "geometry/line.hpp"


namespace uni {


template<class Point>
struct segment : line<Point> {
    using point_type = Point;
    using value_type = typename point_type::value_type;

    constexpr segment(const point_type& p0, const point_type& p1 = point_type()) noexcept(NO_EXCEPT) : line<Point>(p0, p1) {};

    template<class P> constexpr segment(const line<P>& ln) noexcept(NO_EXCEPT) : line<Point>(ln.p0(), ln.p1()) {}
};


template<bool ALLOW_END_POINT, class Point>
bool is_intersecting(const segment<Point>& p, const segment<Point>& q) noexcept(NO_EXCEPT) {
    const auto cp0 = cross(p.p1(), q.p0(), p.p0());
    const auto cp1 = cross(p.p1(), q.p1(), p.p0());
    const auto cp2 = cross(q.p1(), p.p0(), q.p0());
    const auto cp3 = cross(q.p1(), p.p1(), q.p0());

    if(compare(cp0) == 0 and compare(cp1) == 0 and compare(cp2) == 0 and compare(cp3) == 0) {
        return uni::max(p.p0(), q.p0()) <= uni::min(p.p1(), q.p1());
    }

    if constexpr(ALLOW_END_POINT) return compare(cp0 * cp1) <= 0 and compare(cp2 * cp3) <= 0;
    else return compare(cp0 * cp1) < 0 and compare(cp2 * cp3) < 0;
}

template<class Point>
bool is_intersecting(const segment<Point>& p, const segment<Point>& q) noexcept(NO_EXCEPT) {
    return is_intersecting<true>(p, q);
}


template<class Point>
inline constexpr std::optional<Point> intersection(const segment<Point>& p, const segment<Point>& q) noexcept(NO_EXCEPT) {
    if(not is_intersecting(p, q)) return {};
    return intersection(line<Point>(p), line<Point>(q));
}


} // namespace uni
