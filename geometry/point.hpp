#pragma once


#include <complex>
#include <cmath>
#include <iostream>
#include <utility>
#include <type_traits>


#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/exception.hpp"
#include "internal/dev_env.hpp"

#include "view/range.hpp"
#include "view/cyclic.hpp"

#include "utility/functional.hpp"

#include "numeric/float.hpp"


namespace lib {


template <class T>
struct point {
    using value_type = T;

  private:
    value_type _x, _y;

  public:
    constexpr point() : point(0, 0) {}
    constexpr point(const T& x, const T& y) noexcept(NO_EXCEPT) : _x(x), _y(y) {}

    template<class U> constexpr point(const point<U>& p) noexcept(NO_EXCEPT) : _x(p.x()), _y(p.y()) {};
    template<class U> constexpr point(point<U>&& p) noexcept(NO_EXCEPT) : _x(p.x()), _y(p.y()) {};

    template<class U>
    constexpr point& operator=(const point<U>& p) & noexcept(NO_EXCEPT) { this->_x = p._x, this->_y = p._y; return *this; };

    template<class U>
    constexpr point& operator=(point<U>&& p) && noexcept(NO_EXCEPT) { this->_x = p._x, this->_y = p._y; return *this; };

    inline constexpr value_type& x() noexcept(NO_EXCEPT) { return this->_x; }
    inline constexpr value_type& y() noexcept(NO_EXCEPT) { return this->_y; }
    inline constexpr const value_type& x() const noexcept(NO_EXCEPT) { return this->_x; }
    inline constexpr const value_type& y() const noexcept(NO_EXCEPT) { return this->_y; }

    inline constexpr value_type argument() const noexcept(NO_EXCEPT) {
        return static_cast<value_type>(std::arg(std::complex<value_type>(this->x(), this->y())));
    }

    inline constexpr point& operator+=(const point& v) noexcept(NO_EXCEPT) { this->x() += v.x(), this->y() += v.y(); return *this; }
    inline constexpr point& operator-=(const point& v) noexcept(NO_EXCEPT) { this->x() -= v.x(), this->y() -= v.y(); return *this; }

    inline constexpr point& operator+=(const value_type& v) noexcept(NO_EXCEPT) { this->x() += v, this->y() += v; return *this; }
    inline constexpr point& operator-=(const value_type& v) noexcept(NO_EXCEPT) { this->x() -= v, this->y() -= v; return *this; }
    inline constexpr point& operator*=(const value_type& v) noexcept(NO_EXCEPT) { this->x() *= v, this->y() *= v; return *this; }
    inline constexpr point& operator/=(const value_type& v) noexcept(NO_EXCEPT) { this->x() /= v, this->y() /= v; return *this; }

    friend inline constexpr point operator+(const point& p) noexcept(NO_EXCEPT) { return { +p.x(), +p.y() }; }
    friend inline constexpr point operator-(const point& p) noexcept(NO_EXCEPT) { return { -p.x(), -p.y() }; }

    friend inline constexpr point operator+(point a, const point& b) noexcept(NO_EXCEPT) { return a += b; }
    friend inline constexpr point operator-(point a, const point& b) noexcept(NO_EXCEPT) { return a -= b; }
    friend inline constexpr value_type operator*(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() * b.x() + a.y() * b.y(); }

    friend inline constexpr point operator+(point a, const value_type& b) noexcept(NO_EXCEPT) { return a += b; }
    friend inline constexpr point operator-(point a, const value_type& b) noexcept(NO_EXCEPT) { return a -= b; }
    friend inline constexpr point operator*(point a, const value_type& b) noexcept(NO_EXCEPT) { return a *= b; }
    friend inline constexpr point operator/(point a, const value_type& b) noexcept(NO_EXCEPT) { return a /= b; }

    friend inline constexpr point operator+(const value_type& a, point b) noexcept(NO_EXCEPT) { return b += a; }
    friend inline constexpr point operator-(const value_type& a, point b) noexcept(NO_EXCEPT) { return b += a; }
    friend inline constexpr point operator*(const value_type& a, point b) noexcept(NO_EXCEPT) { return b *= a; }
    friend inline constexpr point operator/(const value_type& a, point b) noexcept(NO_EXCEPT) { return b /= a; }

    friend inline constexpr bool operator==(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a.x(), b.x()) == 0 and compare(a.y(), b.y()) == 0; }
    friend inline constexpr bool operator!=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a == b); }

    friend inline constexpr bool operator<(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a.x(), b.x()) != 0 ? compare(a.x(), b.x()) < 0 : compare(a.y(), b.y()) < 0; }
    friend inline constexpr bool operator>(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a.x(), b.x()) != 0 ? compare(a.x(), b.x()) > 0 : compare(a.y(), b.y()) > 0; }
    friend inline constexpr bool operator<=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a > b); }
    friend inline constexpr bool operator>=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a < b); }

    inline std::pair<value_type,value_type> _debug() const { return { this->x(), this->y() }; }
};


template<size_t I, class T>
inline const typename point<T>::value_type& get(const point<T>& p) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return p.x(); }
    else if constexpr(I == 1) { return p.y(); }
    else { static_assert(lib::internal::EXCEPTION<I>); }
}

template<size_t I, class T>
inline typename point<T>::value_type& get(point<T>& p) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return p.x();
    else if constexpr(I == 1) return p.y();
    else static_assert(internal::EXCEPTION<I>);
}

} // namespace lib


namespace std {


template<class T>
struct tuple_size<lib::point<T>> : integral_constant<size_t,2> {};

template<size_t I, class T>
struct tuple_element<I,lib::point<T>> {
    using type = typename lib::point<T>::value_type;
};


template<class T>
inline constexpr T norm(const lib::point<T>& v) noexcept(NO_EXCEPT) { return v.x() * v.x() + v.y() * v.y(); }

template<class T>
inline constexpr T abs(const lib::point<T>& v) noexcept(NO_EXCEPT) {
    if constexpr(is_floating_point_v<T>) {
        return static_cast<T>(std::abs(std::complex<T>(v.x(), v.y())));
    }
    else {
        return static_cast<T>(sqrt(norm(v)));
    }
}

template<class T, class C, class S>
inline basic_istream<C,S>& operator>>(basic_istream<C,S>& in, lib::point<T>& v) noexcept(NO_EXCEPT) {
    T x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class T, class C, class S>
inline basic_ostream<C,S>& operator<<(basic_ostream<C,S>& out, const lib::point<T>& v) noexcept(NO_EXCEPT) {
    out << v.x() << " " << v.y();
    return out;
}


} // namespace std


namespace lib {


template<class T>
inline constexpr T distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::abs(a - b);
}

template<class T>
inline constexpr T squared_distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::norm(a - b);
}

template<class T>
inline constexpr T cross(point<T> a, point<T> b, const point<T>& o = {}) noexcept(NO_EXCEPT) {
    a -= o, b -= o;
    return a.x() * b.y() - a.y() * b.x();
}

template<class T, class Angle = T>
inline constexpr point<T> rotate(const point<T>& p, const Angle angle) noexcept(NO_EXCEPT) {
    return {
        std::cos(angle) * p.x() - std::sin(angle) * p.y(),
        std::sin(angle) * p.x() + std::cos(angle) * p.y()
    };
}

template<class T, class Angle = T>
inline constexpr point<T> rotate(const point<T>& p, const point<T>& q, const Angle angle) noexcept(NO_EXCEPT) {
    return rotate(p - q, angle) + q;
}


template<class T>
inline constexpr positional_relation relation(const point<T>& p, point<T> q, point<T> r) noexcept(NO_EXCEPT) {
    q -= p, r -= p;
    const auto comp_qr = compare(cross(q, r));
    if(comp_qr > 0) return positional_relation::counter_clockwise;
    if(comp_qr < 0) return positional_relation::clockwise;
    if(compare(q * r) < 0) return positional_relation::straight_backward;
    if(compare(std::norm(q), std::norm(r)) < 0) return positional_relation::straight_forward;
    return positional_relation::straight_middle;
};

template<class T>
inline constexpr positional_relation relation(const point<T>& p, const point<T>& q) noexcept(NO_EXCEPT) { return relation<T>({ 0, 0 }, p, q); }


template<bool ALLOW_LINE, class I>
inline bool is_convex(const I first, const I last) noexcept(NO_EXCEPT) {
    const range_view u(first, last);
    const cyclic_view v(u);
    internal::size_t n = u.size();

    REP(i, n) {
        const positional_relation r = relation(v[i], v[i+1], v[i+2]);

        if constexpr(ALLOW_LINE) {
            if(r == positional_relation::clockwise) return false;
        }
        else {
            if(r != positional_relation::anti_clockwise) return false;
        }
    }

    return true;
}

template<class I>
inline bool is_convex(const I first, const I last) noexcept(NO_EXCEPT) { return is_convex<true>(first, last); }

template<bool ALLOW_LINE, class V>
inline bool is_convex(const V& v) noexcept(NO_EXCEPT) { return is_convex<ALLOW_LINE>(std::begin(v), std::end(v)); }

template<class V>
inline bool is_convex(const V& v) noexcept(NO_EXCEPT) { return is_convex<true>(std::begin(v), std::end(v)); }


} // namespace lib
