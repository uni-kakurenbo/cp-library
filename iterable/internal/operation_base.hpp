#pragma once


#include <concepts>
#include <iterator>
#include <sstream>


#include "snippet/aliases.hpp"


namespace lib {


template<std::input_iterator I, std::sentinel_for<I> S>
std::string join(I first, S last, const char* sep = "") noexcept(NO_EXCEPT) {
    if(first == last) return "";
    std::ranges::advance(last, -1);
    std::ostringstream res;
    std::ranges::copy(first, last, std::ostream_iterator<std::iter_value_t<I>>(res, sep));
    res << *last;
    return res.str();
}


template<std::ranges::input_range R>
std::string join(R&& range, const char* sep = "") noexcept(NO_EXCEPT) {
    return join(ALL(range), sep);
}


} // namesapce lib
