#pragma once

#include <type_traits>

#include "algebraic/internal/commutative.hpp"


namespace lib {

namespace algebraic {

namespace internal {


template<class T> using is_commutative = std::is_base_of<commutative,T>;
template<class T> inline constexpr auto is_commutative_v = is_commutative<T>::value;
template<class T> using is_commutative_t = std::enable_if_t<is_commutative_v<T>>;


} // namespace internal

} // namespace algebraic

} // namespace lib
