#pragma once


#include <utility>
#include <tuple>
#include <functional>


#include "geometry/point.hpp"
#include "utility/functional.hpp"


namespace uni {


template<class Point>
struct line {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  protected:
    point_type _p0, _p1;

  public:
    constexpr void normalize() noexcept(NO_EXCEPT) { if(this->_p0 > this->_p1) std::swap(this->_p0, this->_p1); }

    constexpr line() noexcept = default;
    constexpr line(const point_type& p0, const point_type& p1) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1) { this->normalize(); }

    constexpr line(const value_type a, const value_type b, const value_type c) noexcept(NO_EXCEPT) {
        if(compare(a) == 0) this->_p0 = { 0, c / b }, this->_p1 = { 1, c / b };
        else if(compare(b) == 0) this->_p0 = { c / a, 0 }, this->_p1 = { c / a, 1 };
        else this->_p0 = { 0, c / b }, this->_p1 = { c / a, 0 };
        this->normalize();
    }


    inline constexpr auto& p0() noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr auto& p1() noexcept(NO_EXCEPT) { return this->_p1; }

    inline constexpr const auto& p0() const noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr const auto& p1() const noexcept(NO_EXCEPT) { return this->_p1; }

    constexpr auto vertices() noexcept(NO_EXCEPT) {
        return std::tie(this->_p0, this->_p1);
    }

    const constexpr auto vertices() const noexcept(NO_EXCEPT) {
        return std::make_pair(std::cref(this->_p0), std::cref(this->_p1));
    }

    constexpr const auto to_vector() const noexcept(NO_EXCEPT) { return this->_p1 - this->_p0; }

    constexpr const auto length() const noexcept(NO_EXCEPT) { return uni::distance(this->_p0, this->_p1); }
    constexpr const auto squared_length() const noexcept(NO_EXCEPT) { return uni::squared_distance(this->_p0, this->_p1); }

    constexpr const auto midpoint() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 2; }


    constexpr auto projection(const point_type& p) noexcept(NO_EXCEPT) {
        const auto q = this->p0() - this->p1();
        const auto s1 = (p - this->p0()) * q / std::norm(q);
        return this->p0() + q * s1;
    }

    constexpr auto reflection(const point_type& p) noexcept(NO_EXCEPT) {
        return p + (this->projection(p) - p) * 2;
    }


    // implemented in geometry/basic.hpp
    inline constexpr auto relation(const point_type& p) noexcept(NO_EXCEPT);


    auto _debug() const { return std::make_pair(this->_p0, this->_p1); }
};


template<size_t I, class Point>
inline const auto& get(const line<Point>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return ln.p0(); }
    else if constexpr(I == 1) { return ln.p1(); }
    else static_assert(uni::internal::EXCEPTION_ON_VALUE<I>);
}

template<size_t I, class Point>
inline auto& get(line<Point>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return ln.p0();
    else if constexpr(I == 1) return ln.p1();
    else static_assert(uni::internal::EXCEPTION_ON_VALUE<I>);
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
auto& operator>>(basic_istream<C, S>& in, uni::line<Point>& v) noexcept(NO_EXCEPT) {
    Point x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class Point, class C, class S>
auto& operator<<(basic_ostream<C, S>& out, const uni::line<Point>& v) noexcept(NO_EXCEPT) {
    out << v.p0() << " " << v.p1();
    return out;
}


} // namespace std


namespace uni {


template<class Point>
bool parallel(const line<Point>& p, const line<Point>& q) noexcept(NO_EXCEPT) {
    return compare(p.to_vector() * q.to_vector()) == 0;
}


template<class Point>
bool orthogonal(const line<Point>& p, const line<Point>& q) noexcept(NO_EXCEPT) {
    return compare(cross(p.to_vector(), q.to_vector())) == 0;
}

template<class Point>
constexpr std::optional<Point> intersection(const line<Point>& s, const line<Point>& t) noexcept(NO_EXCEPT) {
    using value_type = typename Point::value_type;

    const Point p = s.to_vector(), q = t.to_vector();

    const value_type d0 = cross(p, q);
    const value_type d1 = cross(p, s.p1() - t.p0());

    if(compare(d0) == 0 and compare(d1) == 0) return {};

    return t.p0() + q * (d1 / d0);
}


namespace internal {


template<class T>
constexpr positional_relation relation(
    const line<point<T>>& ln, const point<T>& p,
    T *const _al = nullptr, T *const _bl  = nullptr
) noexcept(NO_EXCEPT) {
    const point<T> a = ln.to_vector();
    const point<T> b = p - ln.p0();

    T al = std::norm(a), bl = std::norm(b);

    if(_al) *_al = al;
    if(_bl) *_bl = bl;

    const T s1 = al * bl;
    const T dot = a * b;

    if(compare(s1, dot * dot) != 0) return positional_relation::out;
    return positional_relation::in;
}


} // namespace internal


template<class Point>
inline constexpr auto line<Point>::relation(const Point& p) noexcept(NO_EXCEPT) {
    return internal::relation(*this, p);
}
template<class Point>
auto distance(const line<Point>& ln, const Point& p) noexcept(NO_EXCEPT) {
    return std::abs(cross(ln.p1(), p, ln.p0())) / distance(ln.p1(), ln.p0());
}

template<class Point>
inline auto distance(const Point& p, const line<Point>& ln) noexcept(NO_EXCEPT) { return distance(ln, p); }


template<class Point>
auto squared_distance(const line<Point>& ln, const Point& p) noexcept(NO_EXCEPT) {
    const auto r = cross(ln.p1(), p, ln.p0());
    return r * r / squared_distance(ln.p1(), ln.p0());
}

template<class Point>
inline auto squared_distance(const Point& p, const line<Point>& ln) noexcept(NO_EXCEPT) { return squared_distance(ln, p); }


} // namespace uni
