#pragma once


#include <iterator>

#include "snippet/iterations.hpp"
#include "internal/types.hpp"
#include "internal/dev_env.hpp"
#include "numeric/prime_enumerator.hpp"


// Thanks to: https://nyaannyaan.github.io/library/multiplicative-function/divisor-multiple-transform.hpp.html
namespace uni {

namespace divisor_transform {


using size_type = internal::size_t;


template<std::size_t OFFSET, std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto zeta(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, uni::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) range[k*p - OFFSET] += range[k - OFFSET];
    }
}

template<std::size_t OFFSET, std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto mobius(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, uni::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) range[k*p - OFFSET] -= range[k - OFFSET];
    }
}


template<std::ranges::sized_range R>
auto zeta(R& range) noexcept(NO_EXCEPT) { return zeta<0>(range); }

template<std::ranges::sized_range R>
auto mobius(R& range) noexcept(NO_EXCEPT) { return mobius<0>(range); }


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


template<std::size_t OFFSET, std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto zeta(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, uni::prime_enumerator(n)) {
        for(size_type k=n/p; k>0; --k) range[k - OFFSET] += range[k*p - OFFSET];
    }
}

template<std::size_t OFFSET, std::ranges::sized_range R>
    requires (!requires { typename R::mapped_type; })
auto mobius(R& range) noexcept(NO_EXCEPT) {
    const auto n = std::ranges::ssize(range);
    ITR(p, uni::prime_enumerator(n)) {
        for(size_type k=1; k*p <= n; ++k) range[k - OFFSET] -= range[k*p - OFFSET];
    }
}


template<std::ranges::sized_range R>
auto zeta(R& range) noexcept(NO_EXCEPT) { return zeta<0>(range); }

template<std::ranges::sized_range R>
auto mobius(R& range) noexcept(NO_EXCEPT) { return mobius<0>(range); }


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

} // namespace uni
