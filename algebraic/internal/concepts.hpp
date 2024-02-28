#pragma once


#include <type_traits>
#include <concepts>


#include "internal/concepts.hpp"

#include "algebraic/base.hpp"


namespace lib {

namespace algebraic {

namespace internal {


template<class T>
concept associative = std::is_base_of_v<algebraic::associative, T>;

template<class T>
concept commutative = std::is_base_of_v<algebraic::commutative, T>;

template<class T>
concept invertible = lib::internal::unary_subtractable<T>;


template<class T>
concept magma = lib::internal::addable<T> && std::same_as<std::common_type_t<T>, T>;

template<class T>
concept semigroup = magma<T> && associative<T>;

template<class T>
concept monoid = semigroup<T> && std::default_initializable<T>;

template<class T>
concept group = monoid<T> && invertible<T>;


} // namespace internal

} // namespace algebraic

} // namespace lib
