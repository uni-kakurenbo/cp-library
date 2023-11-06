#pragma once


#include <cstdint>
#include <iterator>
#include <algorithm>

#include "data_structure/fenwick_tree.hpp"
#include "action/range_sum.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "iterable/compressed.hpp"

#include "snippet/iterations.hpp"


namespace lib {

template<const bool STRICT = true, class T = std::int64_t>
struct inversion {
    template<class I>
    static inline T count(const I first, const I last) noexcept(NO_EXCEPT) {
        const internal::size_t n = std::distance(first, last);
        const auto [ min, max ] = std::minmax_element(first, last);
        const auto m = *max - *min + 1;

        fenwick_tree<actions::range_sum<T>> cnt(m);

        T res = 0;
        {
            internal::size_t i = 0;
            I itr = first;
            for(; i < n; ++i, ++itr) {
                res += cnt(*itr - *min + STRICT, m).fold().val();
                cnt[*itr - *min] += 1;
            }
        }

        return res;
    }

    template<class I>
    static inline T count_with_compression(const I first, const I last) noexcept(NO_EXCEPT) {
        compressed<typename std::iterator_traits<I>::value_type> comp(first, last);
        return inversion::count(std::begin(comp), std::end(comp));
    }
};


} // namespace lib
