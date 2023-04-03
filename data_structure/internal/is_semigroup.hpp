#pragma once

#include <type_traits>
#include "data_structure/semigroup/base.hpp"


namespace lib {

namespace internal {


template<class T> using is_semigroup = std::is_base_of<semigroups::internal::base,T>;
template<class T> inline constexpr auto is_semigroup_v = is_semigroup<T>::value;
template<class T> using is_semigroup_t = std::enable_if_t<is_semigroup_v<T>>;


} // namespace internal

} // namespace lib
