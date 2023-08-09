#pragma once


#include <tuple>


#include "utility/functional.hpp"


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



} // namespace std
