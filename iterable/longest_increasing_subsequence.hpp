#pragma once


#include <algorithm>
#include <iterator>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "structure/grid.hpp"

#include "adaptor/vector.hpp"


namespace uni {


template<bool STRICT, class T, class container = vector<T>>
struct lis : container {
    using size_type = typename internal::size_t;

    std::vector<size_type> indices, positions;

    lis() noexcept = default;

    template<std::ranges::input_range R>
    explicit lis(R&& range) noexcept(NO_EXCEPT) : lis(ALL(range)) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    lis(I first, S last) noexcept(NO_EXCEPT) : positions(std::ranges::distance(first, last), -1) {
        this->reserve(std::ranges::distance(first, last));

        size_type pos = 0;
        for(auto itr=first; itr!=last; ++pos, ++itr) {
            typename container::iterator bound;

            if constexpr(STRICT) bound = std::ranges::lower_bound(*this, *itr);
            else bound = std::ranges::upper_bound(*this, *itr);

            this->positions[pos] = static_cast<size_type>(std::ranges::distance(std::ranges::begin(*this), bound));

            if(std::ranges::end(*this) == bound) this->emplace_back(*itr);
            else *bound = *itr;
        }

        size_type target = std::ranges::max(this->positions);

        for(size_type i = static_cast<size_type>(this->positions.size()); --i >= 0;){
            if(this->positions[i] == target) {
                this->operator[](target) = *(first + i);
                this->indices.emplace_back(i);
                --target;
            }
        }

        std::ranges::reverse(this->indices);
    }
};


} // namespace uni
