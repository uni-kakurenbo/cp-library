#pragma once

#include <type_traits>

#include "algebraic/internal/base.hpp"


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


} // namespace internal

} // namespace algebraic

} // namespace lib
