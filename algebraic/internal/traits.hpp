#pragma once

#include <type_traits>

#include "algebraic/base.hpp"



namespace lib {

namespace algebraic {

namespace internal {


template<class T> using is_magma = std::is_base_of<magma,T>;
template<class T> inline constexpr auto is_magma_v = is_magma<T>::value;
template<class T> using is_magma_t = std::enable_if_t<is_magma_v<T>>;


template<class T> using is_semigroup = std::is_base_of<semigroup,T>;
template<class T> inline constexpr auto is_semigroup_v = is_semigroup<T>::value;
template<class T> using is_semigroup_t = std::enable_if_t<is_semigroup_v<T>>;

template<class T> using is_monoid = std::is_base_of<monoid,T>;
template<class T> inline constexpr auto is_monoid_v = is_monoid<T>::value;
template<class T> using is_monoid_t = std::enable_if_t<is_monoid_v<T>>;

template<class T> using is_group = std::is_base_of<group,T>;
template<class T> inline constexpr auto is_group_v = is_group<T>::value;
template<class T> using is_group_t = std::enable_if_t<is_group_v<T>>;

template<class T> using is_commutative = std::is_base_of<commutative,T>;
template<class T> inline constexpr auto is_commutative_v = is_commutative<T>::value;
template<class T> using is_commutative_t = std::enable_if_t<is_commutative_v<T>>;

template<class T> using is_invertible = std::is_base_of<invertible,T>;
template<class T> inline constexpr auto is_invertible_v = is_invertible<T>::value;
template<class T> using is_invertible_t = std::enable_if_t<is_invertible_v<T>>;


} // namespace internal

} // namespace algebraic

} // namespace lib
