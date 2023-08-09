#pragma once

#ifndef $INT128 // for debugger
#define $INT128

#include <cctype>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>

#include "snippet/internal/types.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"


template<class C, class S>
std::basic_istream<C,S>& operator>>(std::basic_istream<C,S>& in, lib::i128& v) noexcept(NO_EXCEPT) {
    std::string str; in >> str;
    v = 0;
    bool negative = (str[0] == '-');
    REP(d, std::next(str.begin(), negative), str.end()) {
        assert(std::isdigit(*d));
        v = v * 10 + *d - '0';
    }
    if(negative) v *= -1;
    return in;
}


template<class C, class S>
std::basic_istream<C,S>& operator>>(std::basic_istream<C,S>& in, lib::u128& v) noexcept(NO_EXCEPT) {
    std::string str; in >> str;
    v = 0U;
    assert(str[0] != '-');
    REP(d, str.begin(), str.end()) {
        assert(std::isdigit(*d));
        v = v * 10U + *d - '0';
    }
    return in;
}

template<class C, class S>
std::basic_ostream<C,S>& operator<<(std::basic_ostream<C,S>& out, lib::i128 v) noexcept(NO_EXCEPT) {
    if(v == 0) return out << 0;
    if(v < 0) out << '-', v *= -1;
    std::string str;
    while(v > 0) str += static_cast<char>(v%10) + '0', v /= 10;
    std::reverse(str.begin(), str.end());
    return out << str;
}

template<class C, class S>
std::basic_ostream<C,S>& operator<<(std::basic_ostream<C,S>& out, lib::u128 v) noexcept(NO_EXCEPT) {
    if(v == 0) return out << 0U;
    std::string str;
    while(v > 0) str += static_cast<char>(v%10U) + '0', v /= 10U;
    std::reverse(str.begin(), str.end());
    return out << str;
}


#endif //ifndef $INT128
