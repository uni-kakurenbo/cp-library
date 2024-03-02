#pragma once


#include <string>
#include <iterator>
#include <algorithm>


#include "internal/dev_env.hpp"


namespace lib {


template<std::input_iterator I, std::sentinel_for<I> S, class Res = std::string>
Res to_lower(I first, S last) noexcept(NO_EXCEPT) {
    Res res;
    res.reserve(std::distance(first, last));
    std::ranges::transform(first, last, std::back_inserter(res), ::tolower);
    return res;
}

template<std::input_iterator I, std::sentinel_for<I> S, class Res = std::string>
Res to_uppwer(I first, S last) noexcept(NO_EXCEPT) {
    Res res;
    res.reserve(std::distance(first, last));
    std::ranges::transform(first, last, std::back_inserter(res), ::toupper);
    return res;
}


template<class Res = std::string>
Res to_lower(const std::string str) noexcept(NO_EXCEPT) {
    return to_lower<std::string::const_iterator,Res>(std::begin(str), std::end(str));
}

template<class Res = std::string>
Res to_uppwer(const std::string str) noexcept(NO_EXCEPT) {
    return to_uppwer<std::string::const_iterator,Res>(std::begin(str), std::end(str));
}


} // namespace lib
