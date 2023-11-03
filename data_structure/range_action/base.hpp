#pragma once

#include <cstddef>
#include <concepts>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "algebraic/internal/concepts.hpp"


namespace lib {

namespace actions {


template<class operation = std::nullptr_t>
    requires algebraic::internal::monoid<operation> || std::same_as<operation, std::nullptr_t>
struct base {
    static operation fold(const operation& x, const lib::internal::size_t) noexcept(NO_EXCEPT) { return x; }
};


namespace internal {


template<class T>
concept operatable_action = algebraic::internal::monoid<typename T::operand>;

template<class T>
concept effective_action =
    algebraic::internal::monoid<typename T::operation> &&
    requires (const typename T::operand& v, const typename T::operation& f, const lib::internal::size_t length) {
        { T::map(v, f) } -> std::same_as<typename T::operand>;
        { T::fold(f, length) } -> std::same_as<typename T::operation>;
    };

template<class T>
concept full_action = operatable_action<T> && effective_action<T>;

template<class T>
concept operand_only_action = operatable_action<T> && (!effective_action<T>);

template<class T>
concept action = operatable_action<T> || operatable_action<T>;


} // namespace internal

} // namespace actions

} // namespace lib
