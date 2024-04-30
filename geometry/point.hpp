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

#include "utility/functional.hpp"

#include "numeric/float.hpp"


namespace uni {


template <class T>
struct point {
    using value_type = T;

  private:
    value_type _x, _y;

  public:
    constexpr point() : point(0, 0) {}
    constexpr point(const T& x, const T& y) noexcept(NO_EXCEPT) : _x(x), _y(y) {}

    template<class U> constexpr point(const point<U>& p) noexcept(NO_EXCEPT) : _x(p.x()), _y(p.y()) {};

    template<class U>
    constexpr point& operator=(const point<U>& p) & noexcept(NO_EXCEPT) {
        if(&p != this) this->_x = p._x, this->_y = p._y;
        return *this;
    };

    inline constexpr auto& x() noexcept(NO_EXCEPT) { return this->_x; }
    inline constexpr auto& y() noexcept(NO_EXCEPT) { return this->_y; }
    inline constexpr const auto& x() const noexcept(NO_EXCEPT) { return this->_x; }
    inline constexpr const auto& y() const noexcept(NO_EXCEPT) { return this->_y; }

    inline constexpr auto& rotate_quarter() noexcept(NO_EXCEPT) {
        const auto x = this->_x - this->_y;
        const auto y = this->_x + this->_y;
        this->_x = std::move(x), this->_y = std::move(y);
        return *this;
    }

    inline constexpr auto argument() const noexcept(NO_EXCEPT) {
        return static_cast<value_type>(std::arg(std::complex<value_type>(this->_x, this->_y)));
    }

    inline constexpr auto& operator+=(const point& v) noexcept(NO_EXCEPT) { this->_x += v._x, this->_y += v._y; return *this; }
    inline constexpr auto& operator-=(const point& v) noexcept(NO_EXCEPT) { this->_x -= v._x, this->_y -= v._y; return *this; }

    inline constexpr auto& operator+=(const value_type& v) noexcept(NO_EXCEPT) { this->_x += v, this->_y += v; return *this; }
    inline constexpr auto& operator-=(const value_type& v) noexcept(NO_EXCEPT) { this->_x -= v, this->_y -= v; return *this; }
    inline constexpr auto& operator*=(const value_type& v) noexcept(NO_EXCEPT) { this->_x *= v, this->_y *= v; return *this; }
    inline constexpr auto& operator/=(const value_type& v) noexcept(NO_EXCEPT) { this->_x /= v, this->_y /= v; return *this; }

    friend inline constexpr auto operator+(const point& p) noexcept(NO_EXCEPT) { return { +p._x, +p._y }; }
    friend inline constexpr auto operator-(const point& p) noexcept(NO_EXCEPT) { return { -p._x, -p._y }; }

    friend inline constexpr auto operator+(point a, const point& b) noexcept(NO_EXCEPT) { return a += b; }
    friend inline constexpr auto operator-(point a, const point& b) noexcept(NO_EXCEPT) { return a -= b; }
    friend inline constexpr auto operator*(const point& a, const point& b) noexcept(NO_EXCEPT) { return a._x * b._x + a._y * b._y; }

    friend inline constexpr auto operator+(point a, const value_type& b) noexcept(NO_EXCEPT) { return a += b; }
    friend inline constexpr auto operator-(point a, const value_type& b) noexcept(NO_EXCEPT) { return a -= b; }
    friend inline constexpr auto operator*(point a, const value_type& b) noexcept(NO_EXCEPT) { return a *= b; }
    friend inline constexpr auto operator/(point a, const value_type& b) noexcept(NO_EXCEPT) { return a /= b; }

    friend inline constexpr auto operator+(const value_type& a, point b) noexcept(NO_EXCEPT) { return b += a; }
    friend inline constexpr auto operator-(const value_type& a, point b) noexcept(NO_EXCEPT) { return b += a; }
    friend inline constexpr auto operator*(const value_type& a, point b) noexcept(NO_EXCEPT) { return b *= a; }
    friend inline constexpr auto operator/(const value_type& a, point b) noexcept(NO_EXCEPT) { return b /= a; }

    friend inline constexpr bool operator==(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a._x, b._x) == 0 and compare(a._y, b._y) == 0; }
    friend inline constexpr bool operator!=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a == b); }

    friend inline constexpr bool operator<(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a._x, b._x) != 0 ? compare(a._x, b._x) < 0 : compare(a._y, b._y) < 0; }
    friend inline constexpr bool operator>(const point& a, const point& b) noexcept(NO_EXCEPT) { return compare(a._x, b._x) != 0 ? compare(a._x, b._x) > 0 : compare(a._y, b._y) > 0; }
    friend inline constexpr bool operator<=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a > b); }
    friend inline constexpr bool operator>=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a < b); }

    auto _debug() const { return std::make_pair(this->_x, this->_y); }
};


template<size_t I, class T>
inline const auto& get(const point<T>& p) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) { return p.x(); }
    else if constexpr(I == 1) { return p.y(); }
    else static_assert(uni::internal::EXCEPTION<I>);
}

template<size_t I, class T>
inline auto& get(point<T>& p) noexcept(NO_EXCEPT) {
    if constexpr(I == 0) return p.x();
    else if constexpr(I == 1) return p.y();
    else static_assert(internal::EXCEPTION<I>);
}


} // namespace uni


namespace std {


template<class T>
struct tuple_size<uni::point<T>> : integral_constant<size_t,2> {};

template<size_t I, class T>
struct tuple_element<I,uni::point<T>> {
    using type = typename uni::point<T>::value_type;
};


template<class T>
inline constexpr auto norm(const uni::point<T>& v) noexcept(NO_EXCEPT) { return v.x() * v.x() + v.y() * v.y(); }

template<class T>
inline constexpr auto abs(const uni::point<T>& v) noexcept(NO_EXCEPT) {
    if constexpr(is_floating_point_v<T>) {
        return static_cast<T>(std::abs(std::complex<T>(v.x(), v.y())));
    }
    else {
        return static_cast<T>(sqrt(norm(v)));
    }
}

template<class T, class C, class S>
inline auto& operator>>(basic_istream<C, S>& in, uni::point<T>& v) noexcept(NO_EXCEPT) {
    T x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template<class T, class C, class S>
inline auto& operator<<(basic_ostream<C, S>& out, const uni::point<T>& v) noexcept(NO_EXCEPT) {
    out << v.x() << " " << v.y();
    return out;
}


} // namespace std


namespace uni {


template<class T>
inline constexpr auto distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::abs(a - b);
}

template<class T>
inline constexpr auto squared_distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::norm(a - b);
}


template<class T>
inline constexpr auto manhattan_distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
}

template<class T>
inline constexpr auto chebyshev_distance(const point<T>& a, const point<T>& b) noexcept(NO_EXCEPT) {
    return std::max(std::abs(a.x() - b.x()), std::abs(a.y() - b.y()));
}


template<class T>
inline constexpr auto cross(point<T> a, point<T> b, const point<T>& o = point<T>()) noexcept(NO_EXCEPT) {
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
inline constexpr auto rotate(const point<T>& p, const point<T>& q, const Angle angle) noexcept(NO_EXCEPT) {
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
inline constexpr auto relation(const point<T>& p, const point<T>& q) noexcept(NO_EXCEPT) { return relation<T>({ 0, 0 }, p, q); }


template<bool ALLOW_LINE, std::ranges::random_access_range R>
inline bool is_convex(R&& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::size(range);

    REP(i, n) {
        const positional_relation r = relation(range[i], range[uni::mod(i+1,n)], range[uni::mod(i+2,n)]);

        if constexpr(ALLOW_LINE) {
            if(r == positional_relation::clockwise) return false;
        }
        else {
            if(r != positional_relation::anti_clockwise) return false;
        }
    }

    return true;
}

template<std::ranges::random_access_range R>
inline bool is_convex(R&& range) noexcept(NO_EXCEPT) { return is_convex<true>(range); }


} // namespace uni
