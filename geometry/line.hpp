#pragma once


#include <utility>
#include <tuple>
#include <functional>


#include "utility/functional.hpp"

#include "geometry/point.hpp"


namespace uni {


template<class Point>
struct line {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  private:
    point_type _p0, _p1;

  protected:
    inline constexpr void _normalize() noexcept(NO_EXCEPT) { if(this->_p0 > this->_p1) std::swap(this->_p0, this->_p1); }

  public:
    constexpr line() noexcept = default;
    constexpr line(const point_type& p0, const point_type& p1) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1) { this->_normalize(); }

    constexpr line(const value_type a, const value_type b, const value_type c) noexcept(NO_EXCEPT) {
        if(compare(a) == 0) this->_p0 = { 0, c / b }, this->_p1 = { 1, c / b };
        else if(compare(b) == 0) this->_p0 = { c / a, 0 }, this->_p1 = { c / a, 1 };
        else this->_p0 = { 0, c / b }, this->_p1 = { c / a, 0 };
        this->_normalize();
    }

    inline constexpr auto& p0() noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr auto& p1() noexcept(NO_EXCEPT) { return this->_p1; }

    inline constexpr const auto& p0() const noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr const auto& p1() const noexcept(NO_EXCEPT) { return this->_p1; }

    inline constexpr auto vertices() noexcept(NO_EXCEPT) {
        return std::tie(this->p0(), this->p1());
    }
    inline const constexpr auto vertices() const noexcept(NO_EXCEPT) {
        return std::make_pair(std::cref(this->p0()), std::cref(this->p1()));
    }

    inline constexpr const auto to_vector() const noexcept(NO_EXCEPT) { return this->p1() - this->p0(); }

    inline constexpr const auto length() const noexcept(NO_EXCEPT) { return uni::distance(this->p0(), this->p1()); }
    inline constexpr const auto squared_length() const noexcept(NO_EXCEPT) { return uni::squared_distance(this->p0(), this->p1()); }

    inline constexpr const auto midpoint() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 2; }

    inline auto _debug() const { return std::make_pair(this->p0(), this->p1()); }
};


template<size_t I, class Point>
inline const auto& get(const line<Point>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return ln.p0(); }
    else if constexpr(I == 1) { return ln.p1(); }
    else static_assert(uni::internal::EXCEPTION<I>);
}

template<size_t I, class Point>
inline auto& get(line<Point>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return ln.p0();
    else if constexpr(I == 1) return ln.p1();
    else static_assert(uni::internal::EXCEPTION<I>);
}

} // namespace uni


namespace std {


template<class Point>
struct tuple_size<uni::line<Point>> : integral_constant<size_t, 2> {};

template<size_t I, class Point>
struct tuple_element<I, uni::line<Point>> {
    using type = typename uni::line<Point>::value_type;
};


template<class Point, class C, class S>
inline auto& operator>>(basic_istream<C, S>& in, uni::line<Point>& v) noexcept(NO_EXCEPT) {
    Point x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class Point, class C, class S>
inline auto& operator<<(basic_ostream<C, S>& out, const uni::line<Point>& v) noexcept(NO_EXCEPT) {
    out << v.p0() << " " << v.p1();
    return out;
}


} // namespace std


namespace uni {


template<class Point>
bool is_parallel(const line<Point>& p, const line<Point>& q) noexcept(NO_EXCEPT) {
    return compare(p.to_vector() * q.to_vector()) == 0;
}


template<class Point>
bool is_orthogonal(const line<Point>& p, const line<Point>& q) noexcept(NO_EXCEPT) {
    return compare(cross(p.to_vector(), q.to_vector())) == 0;
}

template<class Point>
inline constexpr std::optional<Point> intersection(const line<Point>& s, const line<Point>& t) noexcept(NO_EXCEPT) {
    using value_type = typename Point::value_type;

    const Point p = s.to_vector(), q = t.to_vector();

    const value_type d0 = cross(p, q);
    const value_type d1 = cross(p, s.p1() - t.p0());

    if(compare(d0) == 0 and compare(d1) == 0) return {};

    return t.p0() + q * (d1 / d0);
}


} // namespace uni
