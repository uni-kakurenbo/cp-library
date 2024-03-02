#ifndef INCLUDED_CONCEPTS // for debuggers
#define INCLUDED_CONCEPTS

#include <type_traits>
#include <concepts>
#include <limits>
#include <functional>


namespace lib {

namespace internal {


template<
    template<class...> class Structure,
    class... TemplateParameters
>
concept available_with = std::default_initializable<Structure<TemplateParameters...>>;


template<class T> concept arithmetic = std::is_arithmetic_v<T>;
template<class T> concept pointer = std::is_pointer_v<T>;
template<class T> concept structural = std::is_class_v<T>;


template<class Large, class Small>
concept has_double_digits_of = (std::numeric_limits<Large>::digits == 2 * std::numeric_limits<Small>::digits);


template<class T>
concept has_static_zero = requires { T::zero; };

template<class T>
concept has_static_one = requires { T::one; };


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
concept addition_assignable =
    weakly_addition_assignable<L, R> &&
    weakly_addition_assignable<std::decay_t<std::invoke_result_t<std::plus<>&, L, R>>, R> &&
    weakly_addition_assignable<L, std::invoke_result_t<std::plus<>&, L, R>> &&
    weakly_addition_assignable<std::decay_t<std::invoke_result_t<std::plus<>&, L, R>>, std::invoke_result_t<std::plus<>&, L, R>>;

template<class L, class R = L>
concept subtraction_assignable =
    weakly_subtraction_assignable<L, R> &&
    weakly_subtraction_assignable<std::decay_t<std::invoke_result_t<std::minus<>&, L, R>>, R> &&
    weakly_subtraction_assignable<L, std::invoke_result_t<std::minus<>&, L, R>> &&
    weakly_subtraction_assignable<std::decay_t<std::invoke_result_t<std::minus<>&, L, R>>, std::invoke_result_t<std::minus<>&, L, R>>;

template<class L, class R = L>
concept multipliation_assignalbe =
    weakly_multipliation_assignalbe<L, R> &&
    weakly_multipliation_assignalbe<std::decay_t<std::invoke_result_t<std::multiplies<>&, L, R>>, R> &&
    weakly_multipliation_assignalbe<L, std::invoke_result_t<std::multiplies<>&, L, R>> &&
    weakly_multipliation_assignalbe<std::decay_t<std::invoke_result_t<std::multiplies<>&, L, R>>, std::invoke_result_t<std::multiplies<>&, L, R>>;

template<class L, class R = L>
concept division_assignable =
    weakly_division_assignable<L, R> &&
    weakly_division_assignable<std::decay_t<std::invoke_result_t<std::divides<>&, L, R>>, R> &&
    weakly_division_assignable<L, std::invoke_result_t<std::divides<>&, L, R>> &&
    weakly_division_assignable<std::decay_t<std::invoke_result_t<std::divides<>&, L, R>>, std::invoke_result_t<std::divides<>&, L, R>>;


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

} // namespace lib


#endif //ifndef INCLUDED_CONCEPTS
