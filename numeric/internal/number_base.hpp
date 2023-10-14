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

#include "adapter/string.hpp"
#include "adapter/vector.hpp"


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
    assert(1 < b && b <= std::char_traits<char>::length(CHARS));
    assert(0 <= v);

    lib::string res;
    while(v > 0) {
        res += CHARS[v%b];
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
    std::reverse(ALL(res));

    return res;
}

template<class I, class T = typename std::iterator_traits<I>::value_type>
T from_base_n_sequence(I begin, I end, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    assert(1 < b);

    if(begin == end) return 0;

    T res = 0;
    for(auto itr=begin; itr!=end; ++itr) {
        res *= b;
        res += *itr;
    }

    return res;
}


template<std::ranges::bidirectional_range R, class T = std::ranges::range_value_t<R>>
    requires std::ranges::common_range<R>
T from_base_n_sequence(R range, const lib::internal::size_t b) noexcept(NO_EXCEPT) {
    return from_base_n_sequence(std::ranges::begin(range), std::ranges::end(range), b);
}


} // namespace lib
