#pragma once


#include <cstddef>
#include <vector>

#include "numeric/divisor_multiple_transform.hpp"


namespace lib {


template<class I1, class I2>
auto gcd_convolution(const I1 a_first, const I1 a_last, const I2 b_first, const I2 b_last) {
    std::vector<typename std::iterator_traits<I1>::value_type> a(a_first, a_last);
    std::vector<typename std::iterator_traits<I2>::value_type> b(b_first, b_last);

    assert(a.size() == b.size());

    multiple_transform::zeta(ALL(a)), multiple_transform::zeta(ALL(b));
    for(std::size_t i=0; i<a.size(); ++i) a[i] *= b[i];
    multiple_transform::mobius(ALL(a));
    return a;
}


} // namespace lib
