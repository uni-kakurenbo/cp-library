#pragma once

#include <type_traits>
#include "data_structure/monoid/base.hpp"


namespace lib {

namespace internal {


template<class T> using is_monoid = std::is_base_of<monoid::internal::base,T>;
template<class T> inline constexpr auto is_monoid_v = is_monoid<T>::value;
template<class T> using is_monoid_t = std::enable_if_t<is_monoid_v<T>>;


} // namespace internal

} // namespace lib
