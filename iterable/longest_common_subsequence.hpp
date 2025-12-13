#pragma once


#include <vector>
#include <iterator>
#include <algorithm>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/arithmetic.hpp"

#include "structure/grid.hpp"


namespace uni {


template<class Container = grid<internal::size_t>>
struct lcs_sizes : Container {
    lcs_sizes() noexcept = default;

    template<std::ranges::input_range R0, std::ranges::input_range R1>
    lcs_sizes(R0&& r0, R1&& r1) noexcept(NO_EXCEPT) : lcs_sizes(ALL(r0), ALL(r1)) {}

    template<
        std::input_iterator I0, std::input_iterator I1,
        std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
    >
    lcs_sizes(I0 first0, S0 last0, I1 first1, S1 last1) noexcept(NO_EXCEPT)
      : Container(std::ranges::distance(first0, last0) + 1, std::ranges::distance(first1, last1) + 1)
    {
        internal::size_t pos0 = 0;
        for(auto itr0=first0; itr0!=last0; ++pos0, ++itr0) {
            internal::size_t pos1 = 0;
            for(auto itr1=first1; itr1!=last1; ++pos1, ++itr1) {
                if(*itr0 == *itr1) {
                    (*this)(pos0 + 1, pos1 + 1) = (*this)(pos0, pos1) + 1;
                }
                else {
                    (*this)(pos0 + 1, pos1 + 1) = uni::max((*this)(pos0 + 1, pos1), (*this)(pos0, pos1 + 1));
                }
            }
        }
    }

    template<
        std::ranges::bidirectional_range R0,
        std::ranges::bidirectional_range R1
    >
        requires
            requires (std::remove_cvref_t<R0> r, std::remove_cvref_t<R0>::value_type v) {
                r.push_back(v);
                std::ranges::reverse(r);
            }
    auto restore(R0&& r0, R1&& r1) noexcept(NO_EXCEPT) {
        std::remove_cvref_t<R0> res;

        auto itr0 = std::ranges::rbegin(r0);
        auto itr1 = std::ranges::rbegin(r1);

        const auto last0 = std::ranges::rend(r0);
        const auto last1 = std::ranges::rend(r1);

        auto pos0 = std::ranges::distance(r0) - 1;
        auto pos1 = std::ranges::distance(r1) - 1;

        while(itr0 != last0 && itr1 != last1) {
            if(*itr0 == *itr1) {
                res.push_back(*itr0);
                --pos0, --pos1;
                ++itr0, ++itr1;
            }
            else if((*this)(pos0, pos1) == (*this)(pos0 + 1, pos1)) {
                --pos0, ++itr0;
            }
            else {
                --pos1, ++itr1;
            }
        }

        std::ranges::reverse(res);
        return res;
    }
};


} // namespace uni
