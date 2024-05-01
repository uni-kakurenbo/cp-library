#pragma once


#include <tuple>

#include "utility/functional.hpp"

#include "geometry/point.hpp"
#include "geometry/line.hpp"


namespace uni {


template<class Point>
struct segment : line<Point> {
    using point_type = Point;
    using value_type = typename point_type::value_type;

    constexpr segment() noexcept = default;
    constexpr segment(const point_type& p0, const point_type& p1) noexcept(NO_EXCEPT) : line<Point>(p1) { this->_normalize(); }

    constexpr segment(const line<point_type>& ln) noexcept(NO_EXCEPT) : segment(ln.p0(), ln.p1()) {}

    constexpr auto relation(const point_type& p) noexcept(NO_EXCEPT);
};


template<bool ALLOW_END_POINT, class Point>
bool intersecting(const segment<Point>& s0, const segment<Point>& s1) noexcept(NO_EXCEPT) {
    const auto cp0 = cross(s0.p1(), s1.p0(), s0.p0());
    const auto cp1 = cross(s0.p1(), s1.p1(), s0.p0());
    const auto cp2 = cross(s1.p1(), s0.p0(), s1.p0());
    const auto cp3 = cross(s1.p1(), s0.p1(), s1.p0());

    if(compare(cp0) == 0 and compare(cp1) == 0 and compare(cp2) == 0 and compare(cp3) == 0) {
        return std::max(s0.p0(), s1.p0()) <= std::min(s0.p1(), s1.p1());
    }

    if constexpr(ALLOW_END_POINT) return compare(cp0 * cp1) <= 0 and compare(cp2 * cp3) <= 0;
    else return compare(cp0 * cp1) < 0 and compare(cp2 * cp3) < 0;
}

template<class Point>
bool intersecting(const segment<Point>& s0, const segment<Point>& s1) noexcept(NO_EXCEPT) {
    return intersecting<true>(s0, s1);
}


template<class Point>
inline constexpr std::optional<Point> intersection(const segment<Point>& s0, const segment<Point>& s1) noexcept(NO_EXCEPT) {
    if(!intersecting(s0, s1)) return {};
    return intersection(line<Point>(s0), line<Point>(s1));
}


template<class Point>
constexpr auto segment<Point>::relation(const Point& p) noexcept(NO_EXCEPT) {
    if(p == this->_p0 or p == this->_p1) return positional_relation::on;

    typename Point::value_type al, bl;
    if(internal::relation(p, *this, &al, &bl) == positional_relation::out) {
        return positional_relation::out;
    };

    const auto comp = compare(al, bl);

    if(comp < 0) return positional_relation::out;
    if(comp > 0) return positional_relation::in;

    assert(false);
}


template<class Point>
double distance(const segment<Point>& s0, const segment<Point> &s1) {
    if(intersecting(s0, s1)) return 0;
    return
        min(
            distance(s0, s1.p0()), distance(s0, s1.p1()),
            distance(s1, s0.p0()), distance(s1, s0.p1())
        );
}

template<class Point>
double squared_distance(const segment<Point>& s0, const segment<Point> &s1) {
    if(intersecting(s0, s1)) return 0;
    return
        min(
            squared_distance(s0, s1.p0()), squared_distance(s0, s1.p1()),
            squared_distance(s1, s0.p0()), squared_distance(s1, s0.p1())
        );
}


template<class Point>
auto distance(const segment<Point>& seg, const Point& p) noexcept(NO_EXCEPT) {
    if(compare(dot(seg.p1(), p, seg.p0())) < 0) return distance(p, seg.p0());
    if(compare(dot(seg.p0(), p, seg.p1())) < 0) return distance(p, seg.p1());
    return distance(line<Point>(seg), p);
}

template<class Point>
inline auto distance(const Point& p, const segment<Point>& seg) noexcept(NO_EXCEPT) { return distance(seg, p); }


template<class Point>
auto squared_distance(const segment<Point>& seg, const Point& p) noexcept(NO_EXCEPT) {
    if(compare(dot(seg.p1(), p, seg.p0())) < 0) return squared_distance(p, seg.p0());
    if(compare(dot(seg.p0(), p, seg.p1())) < 0) return squared_distance(p, seg.p1());
    return squared_distance(line<Point>(seg), p);
}

template<class Point>
inline auto squared_distance(const Point& p, const segment<Point>& seg) noexcept(NO_EXCEPT) { return squared_distance(seg, p); }


} // namespace uni
