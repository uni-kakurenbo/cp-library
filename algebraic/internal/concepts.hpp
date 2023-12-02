#pragma once

#include <type_traits>
#include <concepts>

#include "algebraic/base.hpp"

namespace lib {

namespace algebraic {

namespace internal {


template<class T>
concept commutative = std::derived_from<T, algebraic::commutative>;

template<class T>
concept invertible =
    requires(const T& v) {
        { -v } -> std::same_as<T>;
    };


template<class T>
concept magma =
    requires(const T& u, const T& v) {
        { u + v } -> std::same_as<T>;
    };

template<class T>
concept semigroup = magma<T> && std::derived_from<T, associative>;

template<class T>
concept monoid = semigroup<T> && std::default_initializable<T>;

template<class T>
concept group = monoid<T> && invertible<T>;


} // namespace internal

} // namespace algebraic

} // namespace lib
