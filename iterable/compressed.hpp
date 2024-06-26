#pragma once


#include <iterator>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <concepts>


#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/vector.hpp"


namespace uni {

template<class T, class Container = vector<internal::size_t>>
struct compressed : Container {
    using size_type = internal::size_t;
    using value_type = T;

    std::vector<value_type> values;

  public:
    compressed() noexcept(NO_EXCEPT) = default;

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    compressed(I first, S last) noexcept(NO_EXCEPT) {
        this->values.assign(first, last);

        std::ranges::sort(ALL(this->values));

        this->values.erase(std::unique(ALL(this->values)), std::ranges::end(this->values));

        this->resize(std::ranges::distance(first, last));
        {
            auto itr = std::ranges::begin(*this);
            auto e = first;
            for(; e!=last; ++itr, ++e) *itr = this->rank(*e);
        }
    }

    template<std::ranges::input_range R>
    explicit compressed(R&& range) noexcept(NO_EXCEPT) : compressed(ALL(range)) {}

    inline size_type rank_sup() const { return static_cast<size_type>(this->values.size()); }

    inline size_type rank(const value_type& val) const noexcept(NO_EXCEPT) {
        return static_cast<size_type>(
            std::ranges::distance(std::ranges::begin(this->values), std::ranges::lower_bound(this->values, val))
        );
    }

    inline size_type rank2(const value_type& val) const noexcept(NO_EXCEPT) {
        return static_cast<size_type>(
            std::ranges::distance(std::ranges::begin(this->values), std::ranges::upper_bound(this->values, val))
        ) - 1;
    }

    inline value_type value(const size_type rank) const noexcept(NO_EXCEPT) {
        assert(0 <= rank && rank < this->rank_sup());
        return this->values[rank];
    }
};

template<std::input_iterator I, std::sized_sentinel_for<I> S>
explicit compressed(I, S) -> compressed<typename std::iterator_traits<I>::value_type>;

template<std::ranges::input_range R>
explicit compressed(R&&) -> compressed<typename std::ranges::range_value_t<R>>;


} // namespace uni
