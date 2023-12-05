#ifndef INCLUDED_CONCEPTS // for debuggers
#define INCLUDED_CONCEPTS

#include <type_traits>
#include <concepts>


namespace lib {

namespace internal {


template<class T> concept arithmetic = std::is_arithmetic_v<T>;
template<class T> concept pointer = std::is_pointer_v<T>;


template<class Large, class Small>
concept wider_than = (std::numeric_limits<Large>::digits >= 2 * std::numeric_limits<Small>::digits);


template<class L, class R = L>
concept weakly_addable = requires (L lhs, R rhs) { lhs + rhs; };

template<class L, class R = L>
concept weakly_subtractable = requires (L lhs, R rhs) { lhs - rhs; };

template<class L, class R = L>
concept weakly_multipliable = requires (L lhs, R rhs) { lhs * rhs; };

template<class L, class R = L>
concept weakly_divisable = requires (L lhs, R rhs) { lhs / rhs; };


template<class L, class R = L>
concept weakly_addition_assignable = requires (L lhs, R rhs) { lhs += rhs; };

template<class L, class R = L>
concept weakly_subtraction_assignable = requires (L lhs, R rhs) { lhs -= rhs; };

template<class L, class R = L>
concept weakly_multipliation_assignalbe = requires (L lhs, R rhs) { lhs *= rhs; };

template<class L, class R = L>
concept weakly_division_assignable = requires (L lhs, R rhs) { lhs /= rhs; };


template<class T0, class T1>
concept reducible =
    std::common_with<T0, T1> &&
    std::common_with<T0, std::common_type_t<T0, T1>> &&
    std::common_with<std::common_type_t<T0, T1>, T1>;


template<class L, class R = L>
concept addable =
    reducible<L, R> &&
    weakly_addable<L, R> &&
    requires (L lhs, R rhs) { { lhs + rhs } -> std::same_as<std::common_type_t<L, R>>; };

template<class L, class R = L>
concept subtractable =
    reducible<L, R> &&
    weakly_subtractable<L, R> &&
    requires (L lhs, R rhs) { { lhs - rhs } -> std::same_as<std::common_type_t<L, R>>; };

template<class L, class R = L>
concept multipliable =
    reducible<L, R> &&
    weakly_multipliable<L, R> &&
    requires (L lhs, R rhs) { { lhs * rhs } -> std::same_as<std::common_type_t<L, R>>; };

template<class L, class R = L>
concept divisable =
    reducible<L, R> &&
    weakly_divisable<L, R> &&
    requires (L lhs, R rhs) { { lhs / rhs } -> std::same_as<std::common_type_t<L, R>>; };


template<class L, class R = L>
concept addition_assignable =
    weakly_addition_assignable<L, R> &&
    requires (L lhs, R rhs) { { lhs += rhs } -> std::same_as<L&>; };

template<class L, class R = L>
concept subtraction_assignable =
    weakly_subtraction_assignable<L, R> &&
    requires (L lhs, R rhs) { { lhs -= rhs } -> std::same_as<L&>; };

template<class L, class R = L>
concept multipliation_assignalbe =
    weakly_multipliation_assignalbe<L, R> &&
    requires (L lhs, R rhs) { { lhs *= rhs } -> std::same_as<L&>; };

template<class L, class R = L>
concept division_assignable =
    weakly_division_assignable<L, R> &&
    requires (L lhs, R rhs) { { lhs /= rhs } -> std::same_as<L&>; };


template<class T>
concept weakly_incrementable =
    std::movable<T> &&
    requires (T v) {
        { ++v } -> std::same_as<T&>;
        v++;
    };

template<class T>
concept incrementable =
    std::regular<T> &&
    weakly_incrementable<T> &&
    requires (T v) {
        { v++ } -> std::same_as<T>;
    };


template<class L, class R = L>
concept weakly_arithmetic_operable = weakly_addable<L, R> && weakly_subtractable<L, R> && weakly_multipliable<L, R> && weakly_divisable<L, R>;

template<class L, class R = L>
concept weakly_arithmetic_operation_assignable =
    weakly_addition_assignable<L, R> && weakly_subtraction_assignable<L, R> &&
    weakly_multipliation_assignalbe<L, R> && weakly_division_assignable<L, R>;

template<class L, class R = L>
concept arithmetic_operable = addable<L, R> && subtractable<L, R> && multipliable<L, R> && divisable<L, R>;

template<class L, class R = L>
concept arithmetic_operation_assignable =
    addition_assignable<L, R> && subtraction_assignable<L, R> &&
    multipliation_assignalbe<L, R> && division_assignable<L, R>;


template<class T>
concept unary_addable =
    requires (T v) {
        { +v } -> std::same_as<T>;
    };

template<class T>
concept unary_subtractable =
    requires (T v) {
        { -v } -> std::same_as<T>;
    };


template<class T>
concept numeric =
    std::regular<T> &&
    arithmetic_operable<T> &&
    arithmetic_operation_assignable<T> &&
    weakly_incrementable<T> &&
    unary_addable<T> &&
    unary_subtractable<T>;


} // namespace internal

} // namespace lib


#endif //ifndef INCLUDED_CONCEPTS
