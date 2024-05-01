#pragma once


#include <cstdint>
#include <concepts>
#include <utility>
#include <cmath>


#include "snippet/aliases.hpp"

#include "internal/type_traits.hpp"
#include "internal/exception.hpp"

#include "numeric/limits.hpp"


namespace uni {


namespace internal {
    template<class T>
    consteval auto get_pi() {
        if constexpr(std::integral<T>) {
            return static_cast<T>(3);
        }
        else if constexpr(std::same_as<T, float>) {
            return M_PIf;
        }
        else if constexpr(std::same_as<T, double>) {
            return M_PI;
        }
        else if constexpr(std::same_as<T, ld>) {
            return M_PIl;
        }
        else {
            static_assert(EXCEPTION<T>);
        }
    }
} // namespace internal


template<class T = ld>
constexpr auto PI = internal::get_pi<T>();


enum class comparison : std::uint8_t {
    equal_to,
    not_equal_to,

    equals = equal_to,
    eq = equal_to,

    under,
    over,
    or_under,
    or_over,

    less = under,
    more = over,

    less_than = under,
    more_than = over,
    not_less_than = or_over,
    not_more_than = or_under,

    leq = or_under,
    geq = or_over
};


enum class interval_notation : std::uint8_t {
    right_open,
    left_open,
    open,
    closed,
};


enum class replacement_policy : std::uint8_t {
    insert_sync,
    overwrite_sync,
    overwrite_async
};


enum class rotation : std::int8_t {
    clockwise,

    counter_clockwise,
    anti_clockwise = counter_clockwise,
};


enum class positional_relation : std::int8_t {
    clockwise,

    counter_clockwise,
    anti_clockwise = counter_clockwise,

    backward,
    forward,

    in,
    on,
    out,

    included = in,
    inscribed,
    intersecting,
    circumscribed,
    distant,
};


enum class alignment : std::int8_t {
    left,
    center,
    right
};


} // namespace uni
