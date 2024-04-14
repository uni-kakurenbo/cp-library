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
    requires (!requires { typename R::mapped_type; })
auto zeta(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) range[k*p] += range[k];
    }
}

template<std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto mobius(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) range[k*p] -= range[k];
    }
}


template<std::ranges::input_range R>
    requires requires { typename R::mapped_type; }
auto zeta(R& range) noexcept(NO_EXCEPT) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    for(auto itr1 = end; itr1-- != begin; ) {
        for(auto itr2 = begin; itr2 != end; ++itr2) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second += itr2->second;
        }
    }
}

template<std::ranges::input_range R>
    requires requires { typename R::mapped_type; }
auto mobius(R& range) noexcept(NO_EXCEPT) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    for(auto itr2 = begin; itr2 != end; ++itr2) {
        for(auto itr1 = end; (itr1--) != begin; ) {
            if(itr1->first == itr2->first) break;
            if(itr1->first % itr2->first == 0) itr1->second -= itr2->second;
        }
    }
}


} // namespace divisor_transform


namespace multiple_transform  {


using size_type = internal::size_t;


template<std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto zeta(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) range[k] += range[k*p];
    }
}

template<std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto mobius(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, lib::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) range[k] -= range[k*p];
    }
}


template<std::ranges::input_range R>
    requires requires { typename R::mapped_type; }
auto zeta(R& range) noexcept(NO_EXCEPT) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    for(auto itr2 = begin; itr2 != end; ++itr2) {
        for(auto itr1 = end; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second += itr1->second;
        }
    }
}

template<std::ranges::input_range R>
    requires requires { typename R::mapped_type; }
auto mobius(R& range) noexcept(NO_EXCEPT) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);
    for(auto itr2 = end; itr2-- != begin; ) {
        for(auto itr1 = end; --itr1 != itr2; ) {
            if(itr1->first % itr2->first == 0) itr2->second -= itr1->second;
        }
    }
}


} // namespace multiple_transform

} // namespace lib
