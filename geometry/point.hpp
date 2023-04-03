#pragma once

#include <complex>
#include <iostream>
#include <utility>

namespace lib {

template <class T> struct point : std::complex<T> {
  public:
    constexpr point() : std::complex<T>(0) {}

    template <class U> constexpr point(const std::complex<U>& z) : std::complex<T>(z.real(), z.imag()) {}

    template <class U, class V> constexpr point(const U x, const V y) : std::complex<T>(x, y) {}
    template <class U> constexpr point(const point<U>& p) : point(p.x(), p.y()) {}
    template <class U, class V> constexpr point(const std::pair<U,V> z) : point(z.first, z.second) {}

    constexpr T real() = delete;
    constexpr T imag() = delete;

    inline constexpr T x() const { return this->std::complex<T>::real(); }
    inline constexpr T y() const { return this->std::complex<T>::imag(); }

    inline std::pair<T, T> _debug() const { return {this->x(), this->y()}; }
};

template <class T> inline constexpr bool operator<(const point<T>& a, const point<T>& b) {
    return a.x() != b.x() ? a.x() < b.x() : a.y() < b.y();
}
template <class T> inline constexpr bool operator>(const point<T>& a, const point<T>& b) {
    return a.x() != b.x() ? a.x() > b.x() : a.y() > b.y();
}
template <class T> inline constexpr bool operator<=(const point<T>& a, const point<T>& b) {
    return !(a > b);
}
template <class T> inline constexpr bool operator>=(const point<T>& a, const point<T>& b) {
    return !(a < b);
}

template<class T, class C, class S>
std::basic_istream<C,S> &operator>>(std::basic_istream<C, S>& in, point<T>& v) {
    T x, y; in >> x >> y;
    v = { x, y };
    return in;
}

template <class T, class U = T> constexpr T distance(const point<U>& a, const point<U>& b) {
    return std::abs<T>(point<T>(a - b));
}

template <class T> constexpr T cross(const point<T> o, point<T> a, point<T> b) {
    a -= o, b -= o;
    return a.x() * b.y() - a.y() * b.x();
}

} // namespace lib
