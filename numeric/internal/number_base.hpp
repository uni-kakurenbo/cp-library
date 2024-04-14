#pragma once


#include <cassert>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <ranges>
#include <concepts>


#include "internal/types.hpp"
#include "internal/dev_env.hpp"

#include "adaptor/string.hpp"
#include "adaptor/vector.hpp"


namespace lib {


template<std::size_t B, class T>
lib::string to_base_n_string(T v) noexcept(NO_EXCEPT) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static_assert(0 < B and B <= std::char_traits<char>::length(CHARS));
    assert(0 <= v);

    lib::string res;
    while(v > 0) {
        res += CHARS[v%B];
        v /= B;
    }
    std::reverse(ALL(res));

    return res;
}


template<class T>
lib::string to_base_n_string(T v, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    constexpr char CHARS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(1 < b && b <= static_cast<lib::internal::size_t>(std::char_traits<char>::length(CHARS)));
    assert(0 <= v);

    if(v == 0) return "0";

    lib::string res;

    while(v > 0) {
        res += CHARS[v % b];
        v /= b;
    }
    std::reverse(ALL(res));

    return res;
}

template<class T>
lib::vector<T> to_base_n_vector(T v, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    assert(1 < b);
    assert(0 <= v);

    lib::vector<T> res;

    while(v > 0) {
        res.push_back(v%b);
        v /= b;
    }


    return res;
}

template<std::bidirectional_iterator I, class T = typename std::iterator_traits<I>::value_type>
T from_base_n_sequence(I begin, I end, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    assert(1 < b);

    if(begin == end) return 0;

    T res = 0;
    for(auto itr=end; itr-- != begin; ) {
        res *= b;
        res += *itr;
    }

    return res;
}

template<class T, std::forward_iterator I>
T from_base_n_string(I begin, I end, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    assert(1 < b);

    if(begin == end) return 0;

    T sgn = 1;
    if(*begin == '-') {
        sgn = -1;
        ++begin;
    }

    T res = 0;
    for(auto itr=begin; itr != end; ++itr) {
        res *= b;
        if('0' <= *itr && *itr <= '9') {
            res += *itr - '0';
        }
        else if('a' <= *itr && *itr <= 'z') {
            res += *itr - 'a' + 10;
        }
        else if('A' <= *itr && *itr <= 'Z'){
            res += *itr - 'A' + 10;
        }
        else {
            assert(false);
        }
    }

    return res * sgn;
}


template<std::ranges::bidirectional_range R, class T = std::ranges::range_value_t<R>>
    requires std::ranges::common_range<R>
T from_base_n_sequence(R range, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    return from_base_n_sequence(std::ranges::begin(range), std::ranges::end(range), b);
}

template<class T, std::ranges::bidirectional_range R>
    requires std::ranges::common_range<R>
T from_base_n_string(R range, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    return from_base_n_string<T>(std::ranges::begin(range), std::ranges::end(range), b);
}



} // namespace lib
