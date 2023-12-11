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


template<std::ranges::sized_range R>
auto zeta(R& v) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(v);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) v[k*p-1] += v[k-1];
    }
}

template<std::ranges::sized_range R>
auto mobius(R& v) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(v);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) v[k*p-1] -= v[k-1];
    }
}


template<std::ranges::range R>
    requires requires () { typename R::mapped_type; }
auto zeta(R& v) noexcept(NO_EXCEPT) {
    const auto begin = std::ranges::begin(v);
    const auto end = std::ranges::end(v);
    for(auto itr1 = begin; itr1-- != begin; ) {
        for(auto itr2 = begin; itr2 != end; ++itr2) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second += itr2->second;
        }
    }
}

template<std::ranges::range R>
    requires requires () { typename R::mapped_type; }
auto mobius(R& v) noexcept(NO_EXCEPT) {
    const auto begin = std::ranges::begin(v);
    const auto end = std::ranges::end(v);
    for(auto itr2 = begin; itr2 != end; ++itr2) {
        for(auto itr1 = end; (itr1--) != v; ) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second -= itr2->second;
        }
    }
}


} // namespace divisor_transform


namespace multiple_transform  {


using size_type = internal::size_t;


template<std::ranges::sized_range R>
auto zeta(R& v) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(v);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) v[k-1] += v[k*p-1];
    }
}

template<std::ranges::sized_range R>
auto mobius(R& v) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(v);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) v[k-1] -= v[k*p-1];
    }
}


template<std::ranges::range R>
    requires requires () { typename R::mapped_type; }
auto zeta(R& v) noexcept(NO_EXCEPT) {
    const auto begin = std::ranges::begin(v);
    const auto end = std::ranges::end(v);
    for(auto itr2 = begin; itr2 != end; ++itr2) {
        for(auto itr1 = end; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second += itr1->second;
        }
    }
}

template<std::ranges::range R>
    requires requires () { typename R::mapped_type; }
auto mobius(R& v) noexcept(NO_EXCEPT) {
    const auto begin = std::ranges::begin(v);
    const auto end = std::ranges::end(v);
    for(auto itr2 = end; itr2-- != begin; ) {
        for(auto itr1 = end; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second -= itr1->second;
        }
    }
}


} // namespace multiple_transform

} // namespace lib
