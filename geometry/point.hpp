#pragma once


#include <complex>
#include <cmath>
#include <iostream>
#include <utility>

#include "internal/dev_env.hpp"


namespace lib {

template <class T> struct point : protected std::pair<T,T> {
  public:
    constexpr point() {}
    constexpr point(const T& x, const T& y) noexcept(NO_EXCEPT) : std::pair<T,T>(x, y) {}
    template<class U> constexpr point(const point<U>& p) noexcept(NO_EXCEPT) : point(p.x(), p.y()) {};
    template<class U> constexpr point(point<U>&& p) noexcept(NO_EXCEPT) : point(p.x(), p.y()) {};

    template<class U>
    constexpr point& operator=(const point<U>& p) & noexcept(NO_EXCEPT) { this->x() = p.x(),this->y() = p.y(); return *this; };
    template<class U>
    constexpr point& operator=(point<U>&& p) && noexcept(NO_EXCEPT) { this->x() = p.x(), this->y() = p.y(); return *this; };


    inline T& x() noexcept(NO_EXCEPT) { return this->first; }
    inline T& y() noexcept(NO_EXCEPT) { return this->second; }
    inline const T& x() const noexcept(NO_EXCEPT) { return this->first; }
    inline const T& y() const noexcept(NO_EXCEPT) { return this->second; }

    inline constexpr point& operator+=(const point& v) noexcept(NO_EXCEPT) { this->x() += v.x(), this->y() += v.y(); return *this; }
    inline constexpr point& operator-=(const point& v) noexcept(NO_EXCEPT) { this->x() -= v.x(), this->y() -= v.y(); return *this; }

    friend inline constexpr point operator+(point a, const point& b) noexcept(NO_EXCEPT) { return a += b; }
    friend inline constexpr point operator-(point a, const point& b) noexcept(NO_EXCEPT) { return a -= b; }
    friend inline constexpr point operator*(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() * b.x() + a.y() * b.y(); }

    friend inline constexpr bool operator==(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() == b.x() && a.y() == b.y(); }
    friend inline constexpr bool operator!=(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() != b.x() or a.y() != b.y(); }

    friend inline constexpr bool operator<(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() != b.x() ? a.x() < b.x() : a.y() < b.y(); }
    friend inline constexpr bool operator>(const point& a, const point& b) noexcept(NO_EXCEPT) { return a.x() != b.x() ? a.x() > b.x() : a.y() > b.y(); }
    friend inline constexpr bool operator<=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a > b); }
    friend inline constexpr bool operator>=(const point& a, const point& b) noexcept(NO_EXCEPT) { return !(a < b); }

    std::pair<T,T> _debug() const noexcept(NO_EXCEPT) { return { this->x(), this->y() }; }
};


} // namespace lib


template<class T>
inline constexpr T std::abs(const lib::point<T>& v) {
    T x = v.x(), y = v.y(), n = std::max(std::abs(x), std::abs(y));
    if(n == 0) return 0;
    x /= n, y /= n;
    return n * std::sqrt(x * x + y * y);
}

namespace lib {


template<class U, class T>
inline constexpr U distance(const point<T>& a, const point<T>& b) {
    return std::abs(point<U>(a - b));
}

template<class T>
inline constexpr T cross(point<T> a, point<T> b, const point<T>& o = {}) {
    a -= o, b -= o;
    return a.x() * b.y() - a.y() * b.x();
}


} // namespace lib


template<class T, class C, class S>
std::basic_istream<C,S>& operator>>(std::basic_istream<C,S>& in, lib::point<T>& v) {
    T x, y; in >> x >> y;
    v = { x, y };
    return in;
}
