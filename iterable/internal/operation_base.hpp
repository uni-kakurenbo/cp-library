#pragma once


#include <concepts>
#include <iterator>
#include <sstream>
#include <numeric>


#include "snippet/aliases.hpp"


namespace uni {


template<std::input_iterator I, std::sentinel_for<I> S>
std::string join(I first, S last, const char* sep = "") noexcept(NO_EXCEPT) {
    if(first == last) return "";
    std::ostringstream res;
    while(true) {
        res << *first;
        std::ranges::advance(first, 1);
        if(first == last) break;
        res << sep;
    }
    return res.str();
}


template<std::ranges::input_range R>
std::string join(R&& range, const char* sep = "") noexcept(NO_EXCEPT) {
    return join(ALL(range), sep);
}


template<class I, class T = std::iter_value_t<I>>
    requires std::sentinel_for<I, I>
T sum(I first, I last, const T& base = T()) noexcept(NO_EXCEPT) {
    return std::accumulate(first, last, base);
}

template<std::ranges::input_range R, class T = std::ranges::range_value_t<R>>
auto sum(R&& range, T base = T()) noexcept(NO_EXCEPT) {
    auto&& r = range | std::views::common;
    return sum(ALL(r), base);
}


} // namesapce uni
