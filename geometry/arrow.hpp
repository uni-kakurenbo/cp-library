#pragma once


#include <tuple>

#include "utility/functional.hpp"

#include "geometry/point.hpp"
#include "geometry/line.hpp"
#include "geometry/segment.hpp"


namespace uni {


template<class Point>
struct arrow : segment<Point> {
    using point_type = Point;
    using value_type = typename point_type::value_type;

    constexpr arrow() noexcept = default;

    // p0 -> p1
    constexpr arrow(const point_type& p0, const point_type& p1) noexcept(NO_EXCEPT) { this->_p0 = p0, this->_p1 = p1; }

    constexpr auto relation(const point_type& p) noexcept(NO_EXCEPT);
};


template<class Point>
constexpr auto arrow<Point>::relation(const Point& p) noexcept(NO_EXCEPT) {
    if(this->_p0 == p || this->_p1 == p) return positional_relation::on;

    const auto q0 = this->_p1 - this->_p0, q1 = p - this->_p0;

    const auto comp_qr = compare(cross(q0, q1));
    if(comp_qr > 0) return positional_relation::counter_clockwise;
    if(comp_qr < 0) return positional_relation::clockwise;
    if(compare(q0 * q1) < 0) return positional_relation::backward;
    if(compare(std::norm(q0), std::norm(q1)) < 0) return positional_relation::forward;
    return positional_relation::in;
}


} // namespace uni


namespace std {


template<class Point, class C, class S>
auto& operator>>(basic_istream<C, S>& in, uni::arrow<Point>& v) noexcept(NO_EXCEPT) {
    Point x, y; in >> x >> y;
    v = { x, y };
    return in;
}


} // namespace std
