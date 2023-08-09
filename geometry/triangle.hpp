#pragma once


#include <array>
#include <tuple>

#include "internal/dev_env.hpp"

#include "utility/functional.hpp"
#include "geometry/point.hpp"


namespace lib {


template<class Point>
struct triangle {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  private:
    point_type _p0 = {}, _p1 = {}, _p2 = {};

  protected:
    inline constexpr void _normalize() noexcept(NO_EXCEPT) {
        std::array<point_type,3> vs = { this->_p0, this->_p1, this->_p2 };
        std::sort(std::begin(vs), std::end(vs));
        std::tie(this->_p0, this->_p1, this->_p2) = std::tie(vs[0], vs[1], vs[2]);
    }

  public:
    constexpr triangle() noexcept(NO_EXCEPT) {}

    template<class P0, class P1, class P2 = point_type>
    constexpr triangle(const P0& p0, const P1& p1, const P2& p2 = {}) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1), _p2(p2)
    {
        this->_normalize();
    }

    constexpr triangle(const point_type& p0, const point_type& p1, const point_type& p2 = {}) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1), _p2(p2)
    {
        this->_normalize();
    }

    inline constexpr point_type& p0() noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr point_type& p1() noexcept(NO_EXCEPT) { return this->_p1; }
    inline constexpr point_type& p2() noexcept(NO_EXCEPT) { return this->_p2; }

    inline constexpr const point_type& p0() const noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr const point_type& p1() const noexcept(NO_EXCEPT) { return this->_p1; }
    inline constexpr const point_type& p2() const noexcept(NO_EXCEPT) { return this->_p2; }

    inline constexpr auto vertices() noexcept(NO_EXCEPT) {
        return std::tuple<point_type&,point_type&,point_type&>{ this->p0(), this->p1(), this->p2() };
    }
    inline constexpr auto vertices() const noexcept(NO_EXCEPT) {
        return std::tuple<const point_type&, const point_type&, const point_type&>{ this->p0(), this->p1(), this->p2() };
    }

    inline constexpr const value_type area() const noexcept(NO_EXCEPT) {
        return std::abs(cross(this->p0(), this->p1(), this->p2())) / 2;
    }

    inline constexpr std::tuple<value_type,value_type,value_type> distances() const noexcept(NO_EXCEPT) {
        return {
            lib::distance(this->p1(), this->p2()),
            lib::distance(this->p2(), this->p0()),
            lib::distance(this->p0(), this->p1())
        };
    }

    inline constexpr std::tuple<value_type,value_type,value_type> squared_distances() const noexcept(NO_EXCEPT) {
        return {
            lib::squared_distance(this->p1(), this->p2()),
            lib::squared_distance(this->p2(), this->p0()),
            lib::squared_distance(this->p0(), this->p1())
        };
    }

    inline constexpr std::tuple<value_type,value_type,value_type> angles() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return {
            std::acos((d1 * d1 + d2 * d2 - d0 * d0) / (2 * d1 * d2)),
            std::acos((d2 * d2 + d0 * d0 - d1 * d1) / (2 * d2 * d0)),
            std::acos((d0 * d0 + d1 * d1 - d2 * d2) / (2 * d0 * d1))
        };
    }

    inline constexpr  point_type centroid() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 3; }

    inline constexpr point_type circum_center() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->squared_distances();
        const value_type t0 = d0 * (d1 + d2 - d0);
        const value_type t1 = d1 * (d2 + d0 - d1);
        const value_type t2 = d2 * (d0 + d1 - d2);
        return (t0 * this->p0() + t1 * this->p1() + t2 * this->p2()) / (t0 + t1 + t2);
    }

    inline constexpr  point_type inner_center() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return (d0 * this->p0() + d1 * this->p1() + d2 * this->p2()) / (d0 + d1 + d2);
    }

    inline constexpr  point_type ortho_center() const noexcept(NO_EXCEPT) {
        return tuple_sum(this->vertices()) - 2 * this->circum_center();
    }

    inline constexpr  triangle excenters() const noexcept(NO_EXCEPT) {
        const auto [ d0, d1, d2 ] = this->distances();
        return {
            (d1 * this->p1() + d2 * this->p2() - d0 * this->p0()) / (d1 + d2 - d0),
            (d2 * this->p1() + d0 * this->p2() - d1 * this->p0()) / (d2 + d0 - d1),
            (d0 * this->p1() + d1 * this->p2() - d2 * this->p0()) / (d0 + d1 - d2)
        };
    }

    std::tuple<point_type,point_type,point_type> _debug() const noexcept(NO_EXCEPT) {
        return std::tuple{ this->p0(), this->p1(), this->p2() };
    }
};


template<size_t I, class T>
inline const typename triangle<T>::value_type& get(const triangle<T>& t) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return t.p0(); }
    else if constexpr(I == 1) { return t.p1(); }
    else if constexpr(I == 2) { return t.p2(); }
    else { static_assert(internal::EXCEPTION<I>); }
}

template<size_t I, class T>
inline typename triangle<T>::value_type& get(triangle<T>& t) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return t.p0();
    else if constexpr(I == 1) return t.p1();
    else if constexpr(I == 2) return t.p2();
    else static_assert(internal::EXCEPTION<I>);
}


} // namespace lib


namespace std {


template<class T>
struct tuple_size<lib::triangle<T>> : integral_constant<size_t,3> {};

template<size_t I, class T>
struct tuple_element<I,lib::triangle<T>> {
    using type = typename lib::triangle<T>::value_type;
};


template<class T, class C, class S>
basic_istream<C,S>& operator>>(basic_istream<C,S>& in, lib::triangle<T>& v) noexcept(NO_EXCEPT) {
    typename lib::triangle<T>::point_type p, q, r; in >> p >> q >> r;
    v = { p, q, r };
    return in;
}


} // namespace std
