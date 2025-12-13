#pragma once


#include <type_traits>
#include <concepts>
#include <ranges>
#include <limits>
#include <functional>


namespace uni {

namespace internal {


template<class R, class T>
concept convertibel_range = std::convertible_to<std::ranges::range_value_t<R>, T>;


template<class T, class V>
concept item_or_convertible_range = std::convertible_to<T, V> || convertibel_range<T, V>;


template<class Structure>
concept available =
    requires () {
        typename Structure;
    };

template<
    template<class...> class Structure,
    class... TemplateParameters
>
concept available_with = available<Structure<TemplateParameters...>>;


template<class T> concept arithmetic = std::is_arithmetic_v<T>;
template<class T> concept floating_point = std::is_floating_point_v<T>;
template<class T> concept pointer = std::is_pointer_v<T>;
template<class T> concept structural = std::is_class_v<T>;


template<class Large, class Small>
concept has_double_digits_of = (std::numeric_limits<Large>::digits == 2 * std::numeric_limits<Small>::digits);


template<class Large, class Small>
concept has_more_digits_than = (std::numeric_limits<Large>::digits > std::numeric_limits<Small>::digits);

template<class Large, class Small>
concept has_or_more_digits_than = (std::numeric_limits<Large>::digits >= std::numeric_limits<Small>::digits);


template<class T>
concept has_static_zero = requires { T::zero; };

template<class T>
concept has_static_one = requires { T::one; };


template<class L, class R = L>
concept weakly_bitand_calcurable = requires (L lhs, R rhs) { lhs & rhs; };

template<class L, class R = L>
concept weakly_bitor_calcurable = requires (L lhs, R rhs) { lhs | rhs; };

template<class L, class R = L>
concept weakly_bitxor_calcurable = requires (L lhs, R rhs) { lhs ^ rhs; };

template<class L, class R = L>
concept weakly_addable = requires (L lhs, R rhs) { lhs + rhs; };

template<class L, class R = L>
concept weakly_subtractable = requires (L lhs, R rhs) { lhs - rhs; };

template<class L, class R = L>
concept weakly_multipliable = requires (L lhs, R rhs) { lhs * rhs; };

template<class L, class R = L>
concept weakly_divisable = requires (L lhs, R rhs) { lhs / rhs; };

template<class L, class R = L>
concept weakly_remainder_calculable = requires (L lhs, R rhs) { lhs % rhs; };


template<class L, class R = L>
concept weakly_bitand_assignable = requires (L lhs, R rhs) { lhs += rhs; };

template<class L, class R = L>
concept weakly_bitor_assignable = requires (L lhs, R rhs) { lhs |= rhs; };

template<class L, class R = L>
concept weakly_bitxor_assignable = requires (L lhs, R rhs) { lhs ^= rhs; };

template<class L, class R = L>
concept weakly_addition_assignable = requires (L lhs, R rhs) { lhs += rhs; };

template<class L, class R = L>
concept weakly_subtraction_assignable = requires (L lhs, R rhs) { lhs -= rhs; };

template<class L, class R = L>
concept weakly_multipliation_assignalbe = requires (L lhs, R rhs) { lhs *= rhs; };

template<class L, class R = L>
concept weakly_division_assignable = requires (L lhs, R rhs) { lhs /= rhs; };

template<class L, class R = L>
concept weakly_remainder_assignable = requires (L lhs, R rhs) { lhs /= rhs; };


template<class L, class R = L>
concept bitand_calculable =
    weakly_bitand_calcurable<L, R> &&
    weakly_bitand_calcurable<std::invoke_result_t<std::bit_and<>&, L, R>, R> &&
    weakly_bitand_calcurable<L, std::invoke_result_t<std::bit_and<>&, L, R>> &&
    weakly_bitand_calcurable<std::invoke_result_t<std::bit_and<>&, L, R>, std::invoke_result_t<std::bit_and<>&, L, R>>;

template<class L, class R = L>
concept bitor_calculable =
    weakly_bitor_calcurable<L, R> &&
    weakly_bitor_calcurable<std::invoke_result_t<std::bit_or<>&, L, R>, R> &&
    weakly_bitor_calcurable<L, std::invoke_result_t<std::bit_or<>&, L, R>> &&
    weakly_bitor_calcurable<std::invoke_result_t<std::bit_or<>&, L, R>, std::invoke_result_t<std::bit_or<>&, L, R>>;

template<class L, class R = L>
concept bitxor_calculable =
    weakly_bitxor_calcurable<L, R> &&
    weakly_bitxor_calcurable<std::invoke_result_t<std::bit_xor<>&, L, R>, R> &&
    weakly_bitxor_calcurable<L, std::invoke_result_t<std::bit_xor<>&, L, R>> &&
    weakly_bitxor_calcurable<std::invoke_result_t<std::bit_xor<>&, L, R>, std::invoke_result_t<std::bit_xor<>&, L, R>>;

template<class L, class R = L>
concept addable =
    weakly_addable<L, R> &&
    weakly_addable<std::invoke_result_t<std::plus<>&, L, R>, R> &&
    weakly_addable<L, std::invoke_result_t<std::plus<>&, L, R>> &&
    weakly_addable<std::invoke_result_t<std::plus<>&, L, R>, std::invoke_result_t<std::plus<>&, L, R>>;

template<class L, class R = L>
concept subtractable =
    weakly_subtractable<L, R> &&
    weakly_subtractable<std::invoke_result_t<std::minus<>&, L, R>, R> &&
    weakly_subtractable<L, std::invoke_result_t<std::minus<>&, L, R>> &&
    weakly_subtractable<std::invoke_result_t<std::minus<>&, L, R>, std::invoke_result_t<std::minus<>&, L, R>>;

template<class L, class R = L>
concept multipliable =
    weakly_multipliable<L, R> &&
    weakly_multipliable<std::invoke_result_t<std::multiplies<>&, L, R>, R> &&
    weakly_multipliable<L, std::invoke_result_t<std::multiplies<>&, L, R>> &&
    weakly_multipliable<std::invoke_result_t<std::multiplies<>&, L, R>, std::invoke_result_t<std::multiplies<>&, L, R>>;

template<class L, class R = L>
concept divisable =
    weakly_divisable<L, R> &&
    weakly_divisable<std::invoke_result_t<std::divides<>&, L, R>, R> &&
    weakly_divisable<L, std::invoke_result_t<std::divides<>&, L, R>> &&
    weakly_divisable<std::invoke_result_t<std::divides<>&, L, R>, std::invoke_result_t<std::divides<>&, L, R>>;

template<class L, class R = L>
concept remainder_calculable =
    weakly_remainder_calculable<L, R> &&
    weakly_remainder_calculable<std::invoke_result_t<std::modulus<>&, L, R>, R> &&
    weakly_remainder_calculable<L, std::invoke_result_t<std::modulus<>&, L, R>> &&
    weakly_remainder_calculable<std::invoke_result_t<std::modulus<>&, L, R>, std::invoke_result_t<std::modulus<>&, L, R>>;


template<class L, class R = L>
concept bitand_assignable =
    weakly_bitand_assignable<L, R> &&
    weakly_bitand_assignable<std::invoke_result_t<std::bit_and<>&, L, R>, R> &&
    weakly_bitand_assignable<L, std::invoke_result_t<std::bit_and<>&, L, R>> &&
    weakly_bitand_assignable<std::invoke_result_t<std::bit_and<>&, L, R>, std::invoke_result_t<std::bit_and<>&, L, R>>;

template<class L, class R = L>
concept bitor_assignable =
    weakly_bitor_calcurable<L, R> &&
    weakly_bitor_calcurable<std::invoke_result_t<std::bit_or<>&, L, R>, R> &&
    weakly_bitor_calcurable<L, std::invoke_result_t<std::bit_or<>&, L, R>> &&
    weakly_bitor_calcurable<std::invoke_result_t<std::bit_or<>&, L, R>, std::invoke_result_t<std::bit_or<>&, L, R>>;

template<class L, class R = L>
concept bitxor_assignable =
    weakly_bitxor_calcurable<L, R> &&
    weakly_bitxor_calcurable<std::invoke_result_t<std::bit_xor<>&, L, R>, R> &&
    weakly_bitxor_calcurable<L, std::invoke_result_t<std::bit_xor<>&, L, R>> &&
    weakly_bitxor_calcurable<std::invoke_result_t<std::bit_xor<>&, L, R>, std::invoke_result_t<std::bit_xor<>&, L, R>>;

template<class L, class R = L>
concept addition_assignable =
    weakly_addition_assignable<L, R> &&
    weakly_addition_assignable<std::remove_cvref_t<std::invoke_result_t<std::plus<>&, L, R>>, R> &&
    weakly_addition_assignable<L, std::invoke_result_t<std::plus<>&, L, R>> &&
    weakly_addition_assignable<std::remove_cvref_t<std::invoke_result_t<std::plus<>&, L, R>>, std::invoke_result_t<std::plus<>&, L, R>>;

template<class L, class R = L>
concept subtraction_assignable =
    weakly_subtraction_assignable<L, R> &&
    weakly_subtraction_assignable<std::remove_cvref_t<std::invoke_result_t<std::minus<>&, L, R>>, R> &&
    weakly_subtraction_assignable<L, std::invoke_result_t<std::minus<>&, L, R>> &&
    weakly_subtraction_assignable<std::remove_cvref_t<std::invoke_result_t<std::minus<>&, L, R>>, std::invoke_result_t<std::minus<>&, L, R>>;

template<class L, class R = L>
concept multipliation_assignalbe =
    weakly_multipliation_assignalbe<L, R> &&
    weakly_multipliation_assignalbe<std::remove_cvref_t<std::invoke_result_t<std::multiplies<>&, L, R>>, R> &&
    weakly_multipliation_assignalbe<L, std::invoke_result_t<std::multiplies<>&, L, R>> &&
    weakly_multipliation_assignalbe<std::remove_cvref_t<std::invoke_result_t<std::multiplies<>&, L, R>>, std::invoke_result_t<std::multiplies<>&, L, R>>;

template<class L, class R = L>
concept division_assignable =
    weakly_division_assignable<L, R> &&
    weakly_division_assignable<std::remove_cvref_t<std::invoke_result_t<std::divides<>&, L, R>>, R> &&
    weakly_division_assignable<L, std::invoke_result_t<std::divides<>&, L, R>> &&
    weakly_division_assignable<std::remove_cvref_t<std::invoke_result_t<std::divides<>&, L, R>>, std::invoke_result_t<std::divides<>&, L, R>>;

template<class L, class R = L>
concept remainder_assignable =
    weakly_remainder_assignable<L, R> &&
    weakly_remainder_assignable<std::remove_cvref_t<std::invoke_result_t<std::modulus<>&, L, R>>, R> &&
    weakly_remainder_assignable<L, std::invoke_result_t<std::modulus<>&, L, R>> &&
    weakly_remainder_assignable<std::remove_cvref_t<std::invoke_result_t<std::modulus<>&, L, R>>, std::invoke_result_t<std::modulus<>&, L, R>>;


template<class T>
concept weakly_incrementable =
    std::movable<T> &&
    requires (T v) {
        { ++v } -> std::same_as<T&>;
        v++;
    };

template<class T>
concept weakly_decrementable =
    std::movable<T> &&
    requires (T v) {
        { --v } -> std::same_as<T&>;
        v--;
    };


template<class T>
concept incrementable =
    std::regular<T> &&
    weakly_incrementable<T> &&
    requires (T v) {
        { v++ } -> std::same_as<T>;
    };

template<class T>
concept decrementable =
    std::regular<T> &&
    weakly_decrementable<T> &&
    requires (T v) {
        { v-- } -> std::same_as<T>;
    };


template<class L, class R = L>
concept weakly_arithmetic_operable =
    weakly_addable<L, R> &&
    weakly_subtractable<L, R> &&
    weakly_multipliable<L, R> &&
    weakly_divisable<L, R>;

template<class L, class R = L>
concept weakly_arithmetic_operation_assignable =
    weakly_addition_assignable<L, R> &&
    weakly_subtraction_assignable<L, R> &&
    weakly_multipliation_assignalbe<L, R> &&
    weakly_division_assignable<L, R>;

template<class L, class R = L>
concept arithmetic_operable =
    weakly_arithmetic_operable<L, R> &&
    addable<L, R> &&
    subtractable<L, R> &&
    multipliable<L, R> &&
    divisable<L, R>;

template<class L, class R = L>
concept arithmetic_operation_assignable =
    weakly_arithmetic_operation_assignable<L, R> &&
    addition_assignable<L, R> &&
    subtraction_assignable<L, R> &&
    multipliation_assignalbe<L, R> &&
    division_assignable<L, R>;


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

} // namespace uni
