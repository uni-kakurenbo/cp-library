#pragma once

#include <string>
#include <iterator>
#include <numeric>

namespace lib {


template<class I>
std::string join(const I first, const I last, const std::string& sep = "") {
    std::ostringstream res;
    std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<I>::value_type>(res, sep));
    return res.str();
}

template<class V>
std::string join(V& v, const std::string& sep = "") {
    return join(std::begin(v), std::end(v), sep);
}


template<class I, class T = typename std::iterator_traits<I>::value_type>
T sum(const I first, const I second, const T& base = 0) {
    return std::accumulate(first, second, base);
}

template<class V, class T = typename V::value_type>
auto sum(V& v, T base = 0) {
    return sum(std::begin(v), std::end(v), base);
}


} // namespace lib
