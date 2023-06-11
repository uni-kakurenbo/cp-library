#pragma once

#include <iterator>
#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include <numeric>

#include "internal/types.hpp"


namespace lib {


template<class I>
std::string join(const I first, const I last, const std::string& sep = "") noexcept(NO_EXCEPT) {
    std::ostringstream res;
    std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<I>::value_type>(res, sep));
    return res.str();
}

template<class V>
std::string join(const V& v, const std::string& sep = "") noexcept(NO_EXCEPT) {
    return join(std::begin(v), std::end(v), sep);
}


template<class V, class U>
V concat(const V& v, const U& u) noexcept(NO_EXCEPT) {
    V res(std::size(v) + std::size(u));
    std::copy(std::begin(v), std::end(v), std::begin(res));
    std::copy(std::begin(u), std::end(u), std::next(std::begin(res), std::size(v)));
    return res;
}

template<class V, class... Us>
V concat(const V& v, const Us&... tails) noexcept(NO_EXCEPT) {
    return lib::concat(v, lib::concat(tails...));
}


template<class I, class T = typename std::iterator_traits<I>::value_type>
T sum(const I first, const I second, const T& base = 0) noexcept(NO_EXCEPT) {
    return std::accumulate(first, second, base);
}

template<class V, class T = typename V::value_type>
auto sum(const V& v, T base = 0) noexcept(NO_EXCEPT) {
    return sum(std::begin(v), std::end(v), base);
}


template<class I, class T = typename std::iterator_traits<I>::value_type>
T mex(const I first, const I last, const T& base = 0) noexcept(NO_EXCEPT) {
    std::vector<T> val(first, last);
    std::sort(val.begin(), val.end());
    val.erase(std::unique(val.begin(), val.end()), val.end());
    val.erase(val.begin(), std::lower_bound(val.begin(), val.end(), base));

    internal::size_t i = 0;
    while(i < (internal::size_t)val.size() and val[i] == T{i} + base) ++i;

    return T{i} + base;
}

template<class V, class T = typename V::value_type>
auto mex(const V& v, const T& base = 0) noexcept(NO_EXCEPT) {
    return mex(std::begin(v), std::end(v), base);
}


template<class I>
std::vector<std::string> split(const I first, const I last, const typename std::iterator_traits<I>::value_type delim = ' ') {
    std::vector<std::string> res;

    for(auto itr=first, fnd=first; ;itr=std::next(fnd)) {
        fnd = std::find(itr, last, delim);
        res.emplace_back(itr, fnd);
        if(fnd == last) break;
    }

    return res;
}


} // namespace lib
