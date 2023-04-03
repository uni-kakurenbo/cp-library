#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

namespace lib {


template<std::size_t B = 2, class T>
std::string base_n_string(T v) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static_assert(0 < B <= std::strlen(CHARS));
    dev_assert(0 <= v);

    std::string res;
    while(v > 0) {
        res += CHARS[v%B] + '0';
        v /= B;
    }
    std::reverse(ALL(res));

    return res;
}

template<class T>
std::string base_n_string(T v, std::size_t b = 2) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    dev_assert(1 < b && b <= std::strlen(CHARS));
    dev_assert(0 <= v);

    std::string res;
    while(v > 0) {
        res += CHARS[v%b];
        v /= b;
    }
    std::reverse(ALL(res));

    return res;
}

template<class T>
std::vector<T> base_n_vector(T v, std::size_t b = 2) {
    dev_assert(1 < b);
    dev_assert(0 <= v);

    std::vector<T> res;
    while(v > 0) {
        res.push_back(v%b);
        v /= b;
    }
    std::reverse(ALL(res));

    return res;
}


} // namespace lib
