#pragma once


#include <type_traits>

#include "numeric/float.hpp"

#include "geometry/point.hpp"
#include "geometry/segment.hpp"
#include "geometry/triangle.hpp"


namespace uni {


template<class Point, class Radius = typename Point::value_type>
struct circle {
    using point_type = Point;
    using radius_type = Radius;
    using value_type = std::common_type_t<typename point_type::value_type, radius_type>;

  protected:
    point_type _c = 0;
    radius_type _r2 = 0;

  public:
    constexpr circle(const point_type& c, const radius_type& r) noexcept(NO_EXCEPT) : _c(c), _r2(r * r) {}
    constexpr circle(const radius_type& r) noexcept(NO_EXCEPT) : _c({ 0, 0 }), _r2(r * r) {}

    template<class P0, class P1, class P2>
    constexpr circle(const P0& p0, const P1& p1, const P2& p2) noexcept(NO_EXCEPT)
      : circle(triangle(p0, p1, p2))
    {};

    constexpr circle(const point_type& p0, const point_type& p1, const point_type& p2) noexcept(NO_EXCEPT)
      : circle(triangle(p0, p1, p2))
    {};

    template<class P> constexpr circle(const segment<P>& seg) noexcept(NO_EXCEPT)
      : _c(seg.midpoint()), _r2(seg.squared_length() / 4)
    {};

    constexpr circle(const segment<point_type>& seg) noexcept(NO_EXCEPT)
      : _c(seg.midpoint()), _r2(seg.squared_length() / 4)
    {};

    template<class P> constexpr circle(const triangle<P>& t) noexcept(NO_EXCEPT)
      : _c(t.circum_center()), _r2(uni::squared_distance(this->_c, t.p0()))
    {}

    constexpr circle(const triangle<point_type>& t) noexcept(NO_EXCEPT)
      : _c(t.circum_center()), _r2(uni::squared_distance(this->_c, t.p0()))
    {}

    inline constexpr auto& center() noexcept(NO_EXCEPT) { return this->_c; }
    inline auto radius() noexcept(NO_EXCEPT) { return static_cast<value_type>(std::sqrt(this->_r2)); }
    inline constexpr auto& squared_radius() noexcept(NO_EXCEPT) { return this->_r2; }

    inline constexpr const auto& center() const noexcept(NO_EXCEPT) { return this->_c; }
    inline const auto radius() const noexcept(NO_EXCEPT) { return static_cast<value_type>(std::sqrt(this->_r2)); }
    inline constexpr const auto& squared_radius() const noexcept(NO_EXCEPT) { return this->_r2; }

    auto _debug() const noexcept(NO_EXCEPT) { return std::make_pair(this->center(), this->radius()); }
};


} // namespace uni
