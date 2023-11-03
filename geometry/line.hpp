#pragma once


#include <tuple>

#include "utility/functional.hpp"

#include "geometry/point.hpp"


namespace lib {


template<class P>
struct line {
    using point_type = P;
    using value_type = typename point_type::value_type;

  private:
    point_type _p0 = {}, _p1 = {};

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

    inline constexpr const point_type to_vector() const noexcept(NO_EXCEPT) { return this->p1() - this->p0(); }

    inline constexpr const value_type length() const noexcept(NO_EXCEPT) { return lib::distance(this->p0(), this->p1()); }
    inline constexpr const value_type squared_length() const noexcept(NO_EXCEPT) { return lib::squared_distance(this->p0(), this->p1()); }

    inline constexpr const point_type midpoint() const noexcept(NO_EXCEPT) { return tuple_sum(this->vertices()) / 2; }

    inline std::pair<point_type,point_type> _debug() const { return { this->p0(), this->p1() }; }
};


template<size_t I, class T>
inline const typename line<T>::value_type& get(const line<T>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return ln.p0(); }
    else if constexpr(I == 1) { return ln.p1(); }
    else { static_assert(lib::internal::EXCEPTION<I>); }
}

template<size_t I, class T>
inline typename line<T>::value_type& get(line<T>& ln) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return ln.p0();
    else if constexpr(I == 1) return ln.p1();
    else static_assert(lib::internal::EXCEPTION<I>);
}

} // namespace lib


namespace std {


template<class T>
struct tuple_size<lib::line<T>> : integral_constant<size_t,2> {};

template<size_t I, class T>
struct tuple_element<I,lib::line<T>> {
    using type = typename lib::line<T>::value_type;
};


template<class P, class C, class S>
inline basic_istream<C,S>& operator>>(basic_istream<C,S>& in, lib::line<P>& v) noexcept(NO_EXCEPT) {
    P x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class P, class C, class S>
inline basic_ostream<C,S>& operator<<(basic_ostream<C,S>& out, const lib::line<P>& v) noexcept(NO_EXCEPT) {
    out << v.p0() << " " << v.p1();
    return out;
}


} // namespace std


namespace lib {


template<class P>
bool is_parallel(const line<P>& p, const line<P>& q) noexcept(NO_EXCEPT) {
    return compare(p.to_vector() * q.to_vector()) == 0;
}


template<class P>
bool is_orthogonal(const line<P>& p, const line<P>& q) noexcept(NO_EXCEPT) {
    return compare(cross(p.to_vector(), q.to_vector())) == 0;
}

template<class P>
inline constexpr std::optional<P> intersection(const line<P>& s, const line<P>& t) noexcept(NO_EXCEPT) {
    using value_type = typename P::value_type;

    const P p = s.to_vector(), q = t.to_vector();

    const value_type d0 = cross(p, q);
    const value_type d1 = cross(p, s.p1() - t.p0());

    if(compare(d0) == 0 and compare(d1) == 0) return {};

    return t.p0() + q * (d1 / d0);
}


} // namespace lib
