#pragma once


#include <tuple>

#include "utility/functional.hpp"

#include "geometry/point.hpp"


namespace lib {


template<class Point>
struct segment {
    using point_type = Point;
    using value_type = typename point_type::value_type;

  private:
    point_type _p0 = {}, _p1 = {};

  protected:
    inline constexpr void _normalize() noexcept(NO_EXCEPT) { if(this->_p0 > this->_p1) std::swap(this->_p0, this->_p1); }

  public:
    constexpr segment() noexcept = default;
    constexpr segment(const point_type& p0, const point_type& p1 = {}) noexcept(NO_EXCEPT) : _p0(p0), _p1(p1) { this->_normalize(); }

    inline constexpr point_type& p0() noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr point_type& p1() noexcept(NO_EXCEPT) { return this->_p1; }

    inline constexpr const point_type& p0() const noexcept(NO_EXCEPT) { return this->_p0; }
    inline constexpr const point_type& p1() const noexcept(NO_EXCEPT) { return this->_p1; }

    inline constexpr auto vertices() noexcept(NO_EXCEPT) {
        return std::tuple<point_type&,point_type&>{ this->p0(), this->p1() };
    }
    inline const constexpr auto vertices() const noexcept(NO_EXCEPT) {
        return std::tuple<const point_type&, const point_type&>{ this->p0(), this->p1() };
    }

    inline constexpr const value_type length() const noexcept(NO_EXCEPT) { return lib::distance(this->p0(), this->p1()); }
    inline constexpr const value_type squared_length() const noexcept(NO_EXCEPT) { return lib::squared_distance(this->p0(), this->p1()); }

    inline constexpr const point_type midpoint() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 2; }

    inline std::pair<point_type,point_type> _debug() const { return { this->p0(), this->p1() }; }
};


template<size_t I, class T>
inline const typename segment<T>::value_type& get(const segment<T>& seg) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return seg.p0(); }
    else if constexpr(I == 1) { return seg.p1(); }
    else { static_assert(lib::internal::EXCEPTION<I>); }
}

template<size_t I, class T>
inline typename segment<T>::value_type& get(segment<T>& seg) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return seg.p0();
    else if constexpr(I == 1) return seg.p1();
    else static_assert(lib::internal::EXCEPTION<I>);
}

} // namespace lib


namespace std {


template<class T>
struct tuple_size<lib::segment<T>> : integral_constant<size_t,2> {};

template<size_t I, class T>
struct tuple_element<I,lib::segment<T>> {
    using type = typename lib::segment<T>::value_type;
};


template<class P, class C, class S>
inline basic_istream<C,S>& operator>>(basic_istream<C,S>& in, lib::segment<P>& v) noexcept(NO_EXCEPT) {
    P x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class P, class C, class S>
inline basic_ostream<C,S>& operator<<(basic_ostream<C,S>& out, const lib::segment<P>& v) noexcept(NO_EXCEPT) {
    out << v.p0() << " " << v.p1();
    return out;
}


} // namespace std


namespace lib {


template<class P>
bool is_parallel(const segment<P>& p, const segment<P>& q) noexcept(NO_EXCEPT) {
    return (
        compare(
            (p.p0().x() - p.p1().x()) * (q.p0().y() - q.p1().y()),
            (q.p0().x() - q.p1().x()) * (p.p0().y() - p.p1().y())
        ) == 0
    );
}


template<class P>
bool is_intersecting(const segment<P>& p, const segment<P>& q) noexcept(NO_EXCEPT) {
    using value_type = typename P::value_type;

    const value_type cp0 = cross(p.p1(), q.p0(), p.p0());
    const value_type cp1 = cross(p.p1(), q.p1(), p.p0());
    const value_type cp2 = cross(q.p1(), p.p0(), q.p0());
    const value_type cp3 = cross(q.p1(), p.p1(), q.p0());

    if(compare(cp0) == 0 and compare(cp1) == 0 and compare(cp2) == 0 and compare(cp3) == 0) {
        return lib::max(p.p0(), q.p0()) <= lib::min(p.p1(), q.p1());
    }

    return compare(cp0 * cp1) <= 0 and compare(cp2 * cp3) <= 0;
}


} // namespace lib
