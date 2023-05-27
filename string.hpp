#pragma once


#include <string>
#include <iterator>

#include "internal/dev_env.hpp"


namespace lib {


template<class I, class Res = std::string>
Res to_lower(const I first, const I last) noexcept(DEV_ENV) {
    Res res;
    res.reserve(std::distance(first, last));
    std::transform(first, last, std::back_inserter(res), ::tolower);
    return res;
}

template<class I, class Res = std::string>
Res to_uppwer(const I first, const I last) noexcept(DEV_ENV) {
    Res res;
    res.reserve(std::distance(first, last));
    std::transform(first, last, std::back_inserter(res), ::toupper);
    return res;
}


template<class Res = std::string>
Res to_lower(const std::string str) noexcept(DEV_ENV) {
    return to_lower<std::string::const_iterator,Res>(std::begin(str), std::end(str));
}

template<class Res = std::string>
Res to_uppwer(const std::string str) noexcept(DEV_ENV) {
    return to_uppwer<std::string::const_iterator,Res>(std::begin(str), std::end(str));
}


} // namespace lib
