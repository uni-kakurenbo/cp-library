#pragma once


#include <atcoder/convolution>


namespace lib {


using atcoder::convolution;
using atcoder::convolution_ll;


template<class I1, class I2>
auto convolution(const I1 a_first, const I1 a_last, const I2 b_first, const I2 b_last) {
    std::vector<typename std::iterator_traits<I1>::value_type> a(a_first, a_last);
    std::vector<typename std::iterator_traits<I2>::value_type> b(b_first, b_last);
    return atcoder::convolution(a, b);
}

template<class I1, class I2>
auto convolution_ll(const I1 a_first, const I1 a_last, const I2 b_first, const I2 b_last) {
    std::vector<typename std::iterator_traits<I1>::value_type> a(a_first, a_last);
    std::vector<typename std::iterator_traits<I2>::value_type> b(b_first, b_last);
    return atcoder::convolution_ll(a, b);
}


} // namespace lib
