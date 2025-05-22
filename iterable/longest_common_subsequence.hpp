#pragma once


#include <vector>
#include <iterator>
#include <algorithm>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "structure/grid.hpp"


namespace uni {


template<class Container = grid<internal::size_t>, class PrevContainer = grid<spair<internal::size_t>>>
struct lcs_sizes : Container {
    PrevContainer prev;

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
        this->prev.assign(this->height(), this->width(), { -1, -1 });

        internal::size_t pos0 = 0;
        for(auto itr0=first0; itr0!=last0; ++pos0, ++itr0) {
            internal::size_t pos1 = 0;
            for(auto itr1=first1; itr1!=last1; ++pos1, ++itr1) {
                if(*itr0 == *itr1) {
                    (*this)(pos0 + 1, pos1 + 1) = (*this)(pos0, pos1) + 1;
                    this->prev(pos0 + 1, pos1 + 1) = { pos0, pos1 };
                }
                else {
                    const auto& v0 = (*this)(pos0 + 1, pos1), v1 = (*this)(pos0, pos1 + 1);

                    if(v0 > v1) {
                        (*this)(pos0 + 1, pos1 + 1) = v0;
                        this->prev(pos0 + 1, pos1 + 1) = this->prev(pos0 + 1, pos1);
                    }
                    else {
                        (*this)(pos0 + 1, pos1 + 1) = v1;
                        this->prev(pos0 + 1, pos1 + 1) = this->prev(pos0, pos1 + 1);
                    }
                }
            }
        }
    }

    template<std::ranges::input_range R0>
        requires
            requires (std::remove_cvref_t<R0> r, std::remove_cvref_t<R0>::value_type v) {
                r.push_back(v);
                std::ranges::reverse(r);
            }
    auto restore(R0&& r0) {
        std::remove_cvref_t<R0> res;

        auto p = this->prev(-1, -1);
        while(p.first >= 0) {
            res.push_back(r0[p.first]);
            p = this->prev(p);
        }

        std::ranges::reverse(res);
        return res;
    }
};


} // namespace uni
