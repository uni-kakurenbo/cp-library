#pragma once


#include <array>
#include <tuple>
#include <concepts>


#include "internal/dev_env.hpp"
#include "utility/functional.hpp"

#include "geometry/point.hpp"
#include "geometry/circle.hpp"


namespace uni {


template<class Point>
struct triangle {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  private:
    point_type _p0, _p1, _p2;

  protected:
    constexpr void _normalize() noexcept(NO_EXCEPT) {
        std::array<point_type,3> vs = { this->_p0, this->_p1, this->_p2 };
        std::sort(std::begin(vs), std::end(vs));
        std::tie(this->_p0, this->_p1, this->_p2) = std::tie(vs[0], vs[1], vs[2]);
    }

  public:
    constexpr triangle() noexcept(NO_EXCEPT) {}

    constexpr triangle(const point_type& p0, const point_type& p1, const point_type& p2 = point_type()) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1), _p2(p2)
    {
        this->_normalize();
    }


    inline constexpr auto& p0() noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr auto& p1() noexcept(NO_EXCEPT) { return this->_p1; }
    inline constexpr auto& p2() noexcept(NO_EXCEPT) { return this->_p2; }

    inline constexpr const auto& p0() const noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr const auto& p1() const noexcept(NO_EXCEPT) { return this->_p1; }
    inline constexpr const auto& p2() const noexcept(NO_EXCEPT) { return this->_p2; }


    inline constexpr auto vertices() noexcept(NO_EXCEPT) { return std::tie(this->_p0, this->_p1, this->_p2); }
    inline constexpr auto vertices() const noexcept(NO_EXCEPT) { return std::make_tuple(std::cref(this->_p0), std::cref(this->_p1), std::cref(this->_p2)); }


    constexpr const auto signed_area() const noexcept(NO_EXCEPT) { return cross(this->_p0, this->_p1, this->_p2) / 2; }
    inline constexpr const auto area() const noexcept(NO_EXCEPT) { return std::abs(this->signed_area()); }


    constexpr auto distances() const noexcept(NO_EXCEPT) {
        return std::make_tuple(
            uni::distance(this->_p1, this->_p2),
            uni::distance(this->_p2, this->_p0),
            uni::distance(this->_p0, this->_p1)
        );
    }

    constexpr auto squared_distances() const noexcept(NO_EXCEPT) {
        return std::make_tuple(
            uni::squared_distance(this->_p1, this->_p2),
            uni::squared_distance(this->_p2, this->_p0),
            uni::squared_distance(this->_p0, this->_p1)
        );
    }

    constexpr auto angles() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return std::make_tuple(
            std::acos((d1 * d1 + d2 * d2 - d0 * d0) / (2 * d1 * d2)),
            std::acos((d2 * d2 + d0 * d0 - d1 * d1) / (2 * d2 * d0)),
            std::acos((d0 * d0 + d1 * d1 - d2 * d2) / (2 * d0 * d1))
        );
    }


    constexpr  point_type barycenter() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 3; }

    constexpr auto circumcenter() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->squared_distances();
        const auto t0 = d0 * (d1 + d2 - d0);
        const auto t1 = d1 * (d2 + d0 - d1);
        const auto t2 = d2 * (d0 + d1 - d2);
        return (t0 * this->_p0 + t1 * this->_p1 + t2 * this->_p2) / (t0 + t1 + t2);
    }

    constexpr auto incenter() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return (d0 * this->_p0 + d1 * this->_p1 + d2 * this->_p2) / (d0 + d1 + d2);
    }

    constexpr auto orthocenter() const noexcept(NO_EXCEPT) {
        return tuple_sum(this->vertices()) - 2 * this->circumcenter();
    }

    constexpr triangle excenters() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return {
            (d1 * this->_p1 + d2 * this->_p2 - d0 * this->_p0) / (d1 + d2 - d0),
            (d2 * this->_p1 + d0 * this->_p2 - d1 * this->_p0) / (d2 + d0 - d1),
            (d0 * this->_p1 + d1 * this->_p2 - d2 * this->_p0) / (d0 + d1 - d2)
        };
    }


    // implemented in geometry/basic.hpp
    constexpr auto circumcircle() const noexcept(NO_EXCEPT);
    constexpr auto incircle() const noexcept(NO_EXCEPT);


    auto _debug() const noexcept(NO_EXCEPT) {
        return std::make_tuple(this->_p0, this->_p1, this->_p2);
    }
};


template<size_t I, class T>
inline const auto& get(const triangle<T>& t) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return t.p0(); }
    else if constexpr(I == 1) { return t.p1(); }
    else if constexpr(I == 2) { return t.p2(); }
    else static_assert(internal::EXCEPTION<I>);
}

template<size_t I, class T>
inline auto& get(triangle<T>& t) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return t.p0();
    else if constexpr(I == 1) return t.p1();
    else if constexpr(I == 2) return t.p2();
    else static_assert(internal::EXCEPTION<I>);
}


} // namespace uni


namespace std {


template<class T>
struct tuple_size<uni::triangle<T>> : integral_constant<size_t,3> {};

template<size_t I, class T>
struct tuple_element<I,uni::triangle<T>> {
    using type = typename uni::triangle<T>::value_type;
};


template<class T, class C, class S>
auto& operator>>(basic_istream<C, S>& in, uni::triangle<T>& v) noexcept(NO_EXCEPT) {
    typename uni::triangle<T>::point_type p, q, r; in >> p >> q >> r;
    v = { p, q, r };
    return in;
}


} // namespace std


namespace uni {


template<class> struct circle;

template<class Point>
constexpr auto triangle<Point>::incircle() const noexcept(NO_EXCEPT) {
    const auto p = this->incenter();
    return circle<Point>::raw(p, squared_distance(p, line(this->_p0, this->_p1)));
}


template<class Point>
constexpr auto triangle<Point>::circumcircle() const noexcept(NO_EXCEPT) {
    const auto p = this->circumcenter();
    return circle<Point>::raw(p, squared_distance(p, this->_p0));
}


} // namespace uni
