#pragma once


#include <cstdint>
#include <utility>


#include "snippet/aliases.hpp"

#include "internal/type_traits.hpp"

#include "numeric/limits.hpp"
#include "numeric/modint.hpp"


namespace lib {


constexpr i32 INF32 = numeric_limits<i32>::arithmetic_infinity();
constexpr i64 INF64 = numeric_limits<i64>::arithmetic_infinity();


template<class T>
constexpr T EPSILON = numeric_limits<T>::arithmetic_epsilon();

template<class T>
constexpr T INF = numeric_limits<T>::arithmetic_infinity();


template<const unsigned v, const unsigned mod = 998244353>
const lib::static_modint<mod> MINT = v;

template<const unsigned v, const unsigned mod = 998244353>
const unsigned INV = lib::static_modint<mod>{v}.inv().val();

template<const unsigned v, const unsigned mod = 998244353>
const int SINV = lib::static_modint<mod>{v}.inv().val();


constexpr char LN = '\n';
constexpr char SPC = ' ';


constexpr std::pair<int,int> DIRS4[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
constexpr std::pair<int,int> DIRS8[] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };


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

enum class interval : std::uint8_t {
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

    straight_backward,
    straight_forward,
    straight_middle,

    in,
    on,
    out
};


enum class alignment : std::int8_t {
    left,
    center,
    right
};

} // namespace lib
