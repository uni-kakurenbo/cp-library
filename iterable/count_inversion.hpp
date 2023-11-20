#pragma once


#include <cstdint>
#include <iterator>
#include <algorithm>

#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "iterable/compressed.hpp"

#include "data_structure/fenwick_tree.hpp"
#include "action/range_sum.hpp"

namespace lib {

template<const bool STRICT = true, class T = std::int64_t>
struct inversion {
    template<std::input_iterator I, std::sentinel_for<I> S>
    static inline T count(I first, S last) noexcept(NO_EXCEPT) {
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

    template<std::ranges::input_range R>
    static inline T count(R&& range) noexcept(NO_EXCEPT) {
        return inversion::count(ALL(range));
    }

    template<std::input_iterator I, std::sentinel_for<I> S>
    static inline T count_with_compression(I first, S last) noexcept(NO_EXCEPT) {
        compressed<typename std::iterator_traits<I>::value_type> comp(first, last);
        return inversion::count(comp);
    }

    template<std::ranges::input_range R>
    static inline T count_with_compression(R&& range) noexcept(NO_EXCEPT) {
        return inversion::count_with_compression(ALL(range));
    }
};


} // namespace lib
