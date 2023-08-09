#pragma once


#include <functional>
#include <utility>
#include <type_traits>
#include <optional>

#include "internal/dev_env.hpp"
#include "internal/type_traits.hpp"

#include "numeric/arithmetic.hpp"

#include "constants.hpp"


namespace lib {

namespace internal {


template<class T> constexpr T plus(const T a, const T b) noexcept(NO_EXCEPT) { return std::plus<T>{}(a, b); }
template<class T> constexpr T minus(const T a, const T b) noexcept(NO_EXCEPT) { return std::minus<T>{}(a, b); }

template<class T> constexpr T bitxor(const T a, const T b) noexcept(NO_EXCEPT) { return a xor b; }


} // namespace internal


template<class T, class U> inline auto to_optional_if_equal(const T& v, const U& ill) noexcept(NO_EXCEPT) -> std::optional<T> {
    return v == ill ? std::optional<T>{} : std::optional<T>(v);
}
template<class T, class U> inline auto to_optional_if_over(const T& v, const U& ill) noexcept(NO_EXCEPT) -> std::optional<T> {
    return v > ill ? std::optional<T>{} : std::optional<T>(v);
}
template<class T, class U> inline auto to_optional_if_or_over(const T& v, const U& ill) noexcept(NO_EXCEPT) -> std::optional<T> {
    return v >= ill ? std::optional<T>{} : std::optional<T>(v);
}
template<class T, class U> inline auto to_optional_if_under(const T& v, const U& ill) noexcept(NO_EXCEPT) -> std::optional<T> {
    return v < ill ? std::optional<T>{} : std::optional<T>(v);
}
template<class T, class U> inline auto to_optional_if_or_under(const T& v, const U& ill) noexcept(NO_EXCEPT) -> std::optional<T> {
    return v <= ill ? std::optional<T>{} : std::optional<T>(v);
}

template<class T, class F> inline auto to_optional_if(const T& v, F&& f) noexcept(NO_EXCEPT) -> decltype(f(v), std::optional<T>{}){
    return f(v) ? std::optional<T>{} : std::optional<T>(v);
}

template<class T, class U> inline auto mod(const T& x, const U& r) noexcept(NO_EXCEPT) { return (x%r+r)%r; }

template<class T, class U> inline bool chmin(T &a, const U& b) noexcept(NO_EXCEPT) { return (a>b ? a=b, true : false); }
template<class T, class U> inline bool chmax(T &a, const U& b) noexcept(NO_EXCEPT) { return (a<b ? a=b, true : false); }

template<class T, class... Ts> inline bool chmin(T &a, Ts... b) noexcept(NO_EXCEPT) { return chmin(a, min(b...)); }
template<class T, class... Ts> inline bool chmax(T &a, Ts... b) noexcept(NO_EXCEPT) { return chmax(a, max(b...)); }


template<class... Ts>
inline constexpr std::common_type_t<Ts...> tuple_sum(const std::tuple<Ts...>& tuple, const std::common_type_t<Ts...>& base = {}) noexcept(NO_EXCEPT) {
    std::common_type_t<Ts...> res = base;
    tuple_for_each(tuple, [&](const auto& v) constexpr { res += v; });
    return res;
}

template<class... Ts>
inline constexpr std::common_type_t<Ts...> min_element(const std::tuple<Ts...>& tuple) noexcept(NO_EXCEPT) {
    return std::apply([&](auto&&... v) constexpr { return min(v...); }, tuple);
}


template<class... Ts>
inline constexpr std::common_type_t<Ts...> max_element(const std::tuple<Ts...>& tuple) noexcept(NO_EXCEPT) {;
    return std::apply([&](auto&&... v) constexpr { return max(v...); }, tuple);
}


template<interval INTERVAL, class T0, class T1, class T2>
inline constexpr bool in_range(const T0& x, const T1& l, const T2& r) noexcept(NO_EXCEPT) {
    if constexpr(INTERVAL == interval::right_open) return l <= x and x < r;
    else if constexpr(INTERVAL == interval::left_open) return l < x and x <= r;
    else if constexpr(INTERVAL == interval::open) return l < x and x < r;
    return l <= x and x <= r;
}


#if CPP20


template<class F, class Tuple>
constexpr void tuple_for_each(F&& f, Tuple&& tuple) {
    std::apply(
        [&]<class... Ts>(Ts&&... elems) {
            (std::invoke(f, std::forward<Ts>(elems)), ...);
        },
        std::forward<Tuple>(tuple)
    );
}


template<class F, class Tuple>
constexpr auto tuple_transform(F&& f, Tuple&& tuple) {
    return std::apply(
        [&]<class...Ts>(Ts&&... elems) {
            return internal::tuple_or_pair_t<std::invoke_result_t<F&,Ts>...>(
                std::invoke(f, std::forward<Ts>(elems))...
            );
        },
        std::forward<Tuple>(tuple)
    );
}


#else


template<class F, class Tuple>
constexpr void tuple_for_each(F&& f, Tuple&& tuple) {
    std::apply(
        [&](auto&&... elems) {
            (std::invoke(f, std::forward<std::remove_reference_t<decltype(elems)>>(elems)), ...);
        },
        std::forward<Tuple>(tuple)
    );
}


template<class F, class Tuple>
constexpr auto tuple_transform(F&& f, Tuple&& tuple) {
    return std::apply(
        [&](auto&&... elems) {
            return internal::tuple_or_pair_t<std::invoke_result_t<F&,decltype(elems)>...>(
                std::invoke(f, std::forward<std::remove_reference_t<decltype(elems)>>(elems))...
            );
        },
        std::forward<Tuple>(tuple)
    );
}


#endif

} // namespace lib
