#pragma once


#include <iterator>
#include <algorithm>

#include "internal/types.hpp"
#include "internal/types.hpp"


namespace lib {


template<class I, class T = typename std::iterator_traits<I>::value_type>
T mex(const I first, const I last, const T& base = 0) noexcept(DEV_ENV) {
    std::vector<T> val(first, last);
    std::sort(val.begin(), val.end());
    val.erase(std::unique(val.begin(), val.end()), val.end());
    val.erase(val.begin(), std::lower_bound(val.begin(), val.end(), base));

    internal::size_t i = 0;
    while(i < (internal::size_t)val.size() and val[i] == T{i} + base) ++i;

    return T{i} + base;
}

template<class V, class T = typename V::value_type>
auto mex(const V v, const T& base = 0) noexcept(DEV_ENV) {
    return mex(v.begin(), v.end(), base);
}


} // namespace lib
