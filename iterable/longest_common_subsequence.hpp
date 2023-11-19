#pragma once


#include <vector>
#include <iterator>
#include <algorithm>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "structure/grid.hpp"


namespace lib {


template<class container = valgrid<internal::size_t>>
struct lcs_sizes : container {
    explicit lcs_sizes() noexcept(NO_EXCEPT) {}

    template<
        std::input_iterator I0, std::input_iterator I1,
    std::sentinel_for<I0> S0, std::sentinel_for<I1> S1
    >
    lcs_sizes(I0 first0, S0 last0, I1 first1, S1 last1) noexcept(NO_EXCEPT)
      : container(static_cast<internal::size_t>(std::distance(first0, last0))+1, static_cast<internal::size_t>(std::distance(first1, last1))+1) {
        internal::size_t pos0 = 0;
        for(auto itr0=first0; itr0!=last0; ++pos0, ++itr0) {
            internal::size_t pos1 = 0;
            for(auto itr1=first1; itr1!=last1; ++pos1, ++itr1) {
                if(*itr0 == *itr1) (*this)(pos0+1, pos1+1) = (*this)(pos0, pos1) + 1;
                else (*this)(pos0+1, pos1+1) = std::max((*this)(pos0+1, pos1), (*this)(pos0, pos1+1));
            }
        }
    }
};


} // namespace lib
