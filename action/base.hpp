#pragma once


#include <cstddef>
#include <concepts>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/dummy.hpp"

#include "algebraic/internal/concepts.hpp"


namespace uni {

namespace actions {


template<class operation = uni::internal::dummy>
    requires algebraic::internal::monoid<operation> || std::same_as<operation, uni::internal::dummy>
struct base {
    static operation power(const operation& x, const uni::internal::size_t) noexcept(NO_EXCEPT) { return x; }
};


namespace internal {


template<class T>
concept operatable_action = algebraic::internal::magma<typename T::operand>;

template<class T>
concept effective_action =
    algebraic::internal::magma<typename T::operation> &&
    requires (const typename T::operation& f, const uni::internal::size_t length) {
        { T::power(f, length) } -> std::same_as<typename T::operation>;
    };

template<class T>
concept operand_only_action = operatable_action<T> && (!effective_action<T>);

template<class T>
concept effect_only_action = effective_action<T> && (!operatable_action<T>);

template<class T>
concept full_action =
    operatable_action<T> && effective_action<T> &&
    requires (typename T::operation f, typename T::operand v) {
        { T::mapping(f, v) } -> std::same_as<typename T::operand>;
    };

template<class T>
concept action = operatable_action<T> || effective_action<T>;


} // namespace internal

} // namespace actions

} // namespace uni
