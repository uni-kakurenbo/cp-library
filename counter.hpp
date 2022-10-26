#pragma once

#include <iterator>
#include <unordered_map>

template<class I, class T = typename std::iterator_traits<I>::value_type, class C = std::unordered_map<T,int>>
struct Counter : C {
    Counter(const I first, const I last) {
        for(auto itr=first; itr!=last; ++itr) ++(*this)[*itr];
    }
};
