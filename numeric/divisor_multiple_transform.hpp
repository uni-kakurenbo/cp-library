#pragma once


#include <iterator>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"
#include "internal/dev_env.hpp"
#include "numeric/prime_enumerator.hpp"


// Thanks to: https://nyaannyaan.github.io/library/multiplicative-function/divisor-multiple-transform.hpp.html
namespace lib {

namespace divisor_transform {


using size_type = internal::size_t;

template<std::input_iterator I, std::sentinel_for<I> S>
auto zeta(I first, S last) noexcept(NO_EXCEPT) -> decltype((*first) += (*first), void()) {
    const auto n = static_cast<size_type>(std::distance(first, last));
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) first[k*p-1] += first[k-1];
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto mobius(I first, S last) noexcept(NO_EXCEPT) -> decltype((*first) += (*first), void()) {
    const auto n = static_cast<size_type>(std::distance(first, last));
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) first[k*p-1] -= first[k-1];
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto zeta(I first, S last) noexcept(NO_EXCEPT) -> decltype(first->first, first->second, void()) {
    for(auto itr1 = last; itr1-- != first; ) {
        for(auto itr2 = first; itr2 != last; ++itr2) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second += itr2->second;
        }
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto mobius(I first, S last) noexcept(NO_EXCEPT) -> decltype(first->first, first->second, void()) {
    for(auto itr2 = first; itr2 != last; ++itr2) {
        for(auto itr1 = last; (itr1--) != first; ) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second -= itr2->second;
        }
    }
}


} // namespace divisor_transform


namespace multiple_transform  {


using size_type = internal::size_t;

template<std::input_iterator I, std::sentinel_for<I> S>
auto zeta(I first, S last) noexcept(NO_EXCEPT) -> decltype((*first) += (*first), void()) {
    const auto n = static_cast<size_type>(std::distance(first, last));
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) first[k-1] += first[k*p-1];
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto mobius(I first, S last) noexcept(NO_EXCEPT) -> decltype((*first) += (*first), void()) {
    const auto n = static_cast<size_type>(std::distance(first, last));
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) first[k-1] -= first[k*p-1];
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto zeta(I first, S last) noexcept(NO_EXCEPT) -> decltype(first->first, first->second, void()) {
    for(auto itr2 = first; itr2 != last; ++itr2) {
        for(auto itr1 = last; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second += itr1->second;
        }
    }
}

template<std::input_iterator I, std::sentinel_for<I> S>
auto mobius(I first, S last) noexcept(NO_EXCEPT) -> decltype(first->first, first->second, void()) {
    for(auto itr2 = last; itr2-- != first; ) {
        for(auto itr1 = last; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second -= itr1->second;
        }
    }
}


} // namespace multiple_transform

} // namespace lib
