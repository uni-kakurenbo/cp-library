#pragma once


#include <optional>
#include <type_traits>

#include "numeric/float.hpp"

#include "geometry/point.hpp"
#include "geometry/triangle.hpp"
#include "geometry/segment.hpp"
#include "geometry/circle.hpp"


namespace lib {


template<class Point, class Segment>
inline constexpr bool is_on(const Point p, const Segment seg) noexcept(NO_EXCEPT) {
    using value_type = std::common_type_t<typename Point::value_type, typename Segment::value_type>;
    const point<value_type> a = seg.p1() - seg.p0();
    const point<value_type> b = p - seg.p0();
    const value_type al = std::abs(a), bl = std::abs(b);
    const value_type t = al * bl;
    const value_type dot = a * b;
    if(compare(t, dot) != 0) return false;
    return compare(al, bl) > 0;
}


template<class Point>
inline std::optional<Point> intersection(segment<Point> p, segment<Point> q) {
    using value_type = typename Point::value_type;

    const value_type det = (p.p0().x() - p.p1().x()) * (q.p1().y() - q.p0().y()) - (q.p1().x() - q.p0().x()) * (p.p0().y() - p.p1().y());
    if(compare(det) == 0) return {};

    const value_type t = ((q.p1().y() - q.p0().y()) * (q.p1().x() - p.p1().x()) + (q.p0().x() - q.p1().x()) * (q.p1().y() - p.p1().y())) / det;

    const Point res(
        t * p.p0().x() + (value_type{1} - t) * p.p1().x(),
        t * p.p0().y() + (value_type{1} - t) * p.p1().y()
    );

    if(not is_on(res, p)) return {};
    if(not is_on(res, q)) return {};

    return res;
}


} // namespace lib
