#pragma once


#include <iterator>
#include <ranges>


#include "internal/types.hpp"
#include "internal/type_traits.hpp"


namespace uni {


template<class Comparator, std::input_iterator I, std::sentinel_for<I> S>
constexpr std::optional<internal::size_t> cyclically_ordered(I first, S end) noexcept(NO_EXCEPT) {
    constexpr auto comparator = Comparator{};

    internal::size_t res = -1;

    auto itr = first, prev = first;
    for(internal::size_t i = 0; ++i, ++itr != end; ++prev) {
        if(!comparator(*prev, *itr)) {
            if(res >= 0) return {};
            else res = i;
        }
    }

    if(!comparator(*prev, *first)) {
        if(res >= 0) return {};
        else return 0;
    }

    return res;
}


template<class Comparator, std::ranges::input_range R>
constexpr auto cyclically_ordered(R&& vals) noexcept(NO_EXCEPT) {
    return cyclically_ordered<Comparator>(ALL(vals));
}


template<std::ranges::input_range R>
constexpr auto cyclically_ordered(R&& vals) noexcept(NO_EXCEPT) {
    return cyclically_ordered<std::less<>>(ALL(vals));
}


template<class Comparator, class T>
constexpr auto cyclically_ordered(std::initializer_list<T> vals) noexcept(NO_EXCEPT) {
    return cyclically_ordered<Comparator>(ALL(vals));
}

template<class T>
constexpr auto cyclically_ordered(std::initializer_list<T> vals) noexcept(NO_EXCEPT) {
    return cyclically_ordered<std::less<>>(ALL(vals));
}


template<class Comparator, class... Ts>
    requires std::relation<Comparator, std::common_type_t<Ts...>, std::common_type_t<Ts...>>
constexpr auto cyclically_ordered(Ts&&... ps) noexcept(NO_EXCEPT) {
    return cyclically_ordered<Comparator, std::common_type_t<Ts...>>({ std::forward<Ts>(ps)... });
}

template<class... Ts>
constexpr auto cyclically_ordered(Ts&&... ps) noexcept(NO_EXCEPT) {
    return cyclically_ordered<std::less<>, std::common_type_t<Ts...>>({ std::forward<Ts>(ps)... });
}


} // namespace uni
