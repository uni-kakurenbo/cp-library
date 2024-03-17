#pragma once


#include <concepts>
#include <iterator>
#include <sstream>


#include "snippet/aliases.hpp"


namespace lib {


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


} // namesapce lib
