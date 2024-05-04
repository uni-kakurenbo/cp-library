#pragma once


#include <type_traits>


#include "global/constants.hpp"
#include "numeric/float.hpp"

#include "geometry/point.hpp"
#include "geometry/segment.hpp"


namespace uni {


template<class Point>
struct circle {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  protected:
    point_type _c;
    value_type _r2;

  public:
    circle() noexcept = default;

    static constexpr auto raw(const point_type& c, const value_type& r2) noexcept(NO_EXCEPT) {
        circle res; res._c = c, res._r2 = r2;
        return res;
    }


    constexpr circle(const point_type& c, const value_type& r) noexcept(NO_EXCEPT) : _c(c), _r2(r * r) {}
    explicit constexpr circle(const value_type& r) noexcept(NO_EXCEPT) : _c({ 0, 0 }), _r2(r * r) {}


    constexpr circle(const point_type& p0, const point_type& p1, const point_type& p2) noexcept(NO_EXCEPT)
      : circle(triangle(p0, p1, p2))
    {};


    explicit constexpr circle(const segment<point_type>& seg) noexcept(NO_EXCEPT)
      : _c(seg.midpoint()), _r2(seg.squared_length() / 4)
    {};


    inline constexpr auto& center() noexcept(NO_EXCEPT) { return this->_c; }
    inline constexpr const auto& center() const noexcept(NO_EXCEPT) { return this->_c; }

    inline auto radius() const noexcept(NO_EXCEPT) { return static_cast<value_type>(std::sqrt(this->_r2)); }

    inline constexpr auto& squared_radius() noexcept(NO_EXCEPT) { return this->_r2; }
    inline constexpr const auto& squared_radius() const noexcept(NO_EXCEPT) { return this->_r2; }


    constexpr auto relation(const point_type& p) noexcept(NO_EXCEPT) {
        const auto dist = uni::squared_distance(this->_c, p);
        const auto comp = compare(dist, this->_r2);
        if(comp > 0) return positional_relation::out;
        if(comp < 0) return positional_relation::in;
        return positional_relation::on;
    }


    auto _debug() const noexcept(NO_EXCEPT) { return std::make_pair(this->center(), this->radius()); }
};


template<class Point>
int count_common_tangent(const circle<Point>& c0, const circle<Point>& c1) {
    const auto dist = distance(c0.center(), c1.center());
    const auto l0 = c0.radius() + c1.radius();
    const auto l1 = std::abs(c0.radius() - c1.radius());
    if(compare(dist, l0) > 0) return 4;
    if(compare(dist, l0) == 0) return 3;
    if(compare(dist, l1) == 0) return 1;
    if(compare(dist, l1) < 0) return 0;
    return 2;
}

template<class Point>
auto relation(const circle<Point>& c0, const circle<Point>& c1) {
    const auto t = count_common_tangent(c0, c1);

    switch(t) {
        case 0: return uni::positional_relation::included;
        case 1: return uni::positional_relation::inscribed;
        case 2: return uni::positional_relation::intersecting;
        case 3: return uni::positional_relation::circumscribed;
        case 4: return uni::positional_relation::distant;
    }

    assert(false);
}


} // namespace uni
