#pragma once

#include <type_traits>
#include "data_structure/range_action/base.hpp"


namespace lib {

namespace internal {


template<class T> using is_action = std::is_base_of<action::internal::base,T>;
template<class T> inline constexpr auto is_action_v = is_action<T>::value;
template<class T> using is_action_t = std::enable_if_t<is_action_v<T>>;


} // namespace internal

} // namespace lib
