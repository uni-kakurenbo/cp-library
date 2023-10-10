#pragma once


#include <optional>
#include <type_traits>

#include "numeric/float.hpp"

#include "geometry/point.hpp"
#include "geometry/triangle.hpp"
#include "geometry/segment.hpp"
#include "geometry/circle.hpp"


namespace lib {


template<class T>
inline constexpr point<T> projected(const point<T>& p, const line<point<T>>& ln) noexcept(NO_EXCEPT) {
    const point<T> q = ln.p0() - ln.p1();
    const T t = (p - ln.p0()) * q / std::norm(q);
    return ln.p0() + q * t;
}


template<class T>
inline constexpr point<T> reflected(const point<T>& p, const line<point<T>>& ln) {
    return p + (projection(ln, p) - p) * 2;
}


template<class T>
inline constexpr positional_relation relation(const point<T>& p, const circle<T>& o) noexcept(NO_EXCEPT) {
    const T dist = lib::squared_distance(o.center(), p);
    const auto comp = compare(dist, o.squared_radius());
    if(comp > 0) return positional_relation::out;
    if(comp < 0) return positional_relation::in;
    return positional_relation::on;
}

namespace internal {


template<class T>
inline constexpr positional_relation relation(
    const point<T>& p, const line<point<T>>& ln,
    T *const _al = nullptr, T *const _bl  = nullptr
) noexcept(NO_EXCEPT) {
    const point<T> a = ln.to_vector();
    const point<T> b = p - ln.p0();

    T al = std::norm(a), bl = std::norm(b);

    if(_al) *_al = al;
    if(_bl) *_bl = bl;

    const T t = al * bl;
    const T dot = a * b;

    if(compare(t, dot * dot) != 0) return positional_relation::out;
    return positional_relation::in;
}


} // namespace internal



template<class T>
inline constexpr positional_relation relation(const point<T>& p, const line<point<T>>& seg) noexcept(NO_EXCEPT) {
    return internal::relation(p, seg);
}


template<class T>
inline constexpr positional_relation relation(const point<T>& p, const segment<point<T>>& seg) noexcept(NO_EXCEPT) {
    if(p == seg.p0() or p == seg.p1()) return positional_relation::on;

    T al, bl;
    internal::relation(p, seg, &al, &bl);

    const auto comp = compare(al, bl);

    if(comp < 0) return positional_relation::out;
    if(comp > 0) return positional_relation::in;

    assert(false);
}


} // namespace lib
