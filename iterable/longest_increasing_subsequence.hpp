#pragma once


#include <algorithm>
#include <iterator>

#include "snippet/aliases.hpp"
#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "structure/grid.hpp"

#include "adapter/vector.hpp"


namespace lib {


template<bool STRICT, class T = i64, class container = vector<T>>
struct lis : container {
    using size_type = typename internal::size_t;

    std::vector<size_type> indices;

    explicit lis() noexcept(NO_EXCEPT) {}

    template<class I>
    explicit lis(const I first, const I last) noexcept(NO_EXCEPT) {
        std::vector<size_type> positions(std::distance(first, last), -1);

        size_type pos = 0;
        for(auto itr=first; itr!=last; ++pos, ++itr) {
            typename container::iterator bound;

            if constexpr(STRICT) bound = std::lower_bound(std::begin(*this), std::end(*this), *itr);
            else bound = std::upper_bound(std::begin(*this), std::end(*this), *itr);

            positions[pos] = static_cast<size_type>(std::distance(std::begin(*this), bound));

            if(std::end(*this) == bound) this->emplace_back(*itr);
            else *bound = *itr;
        }

        size_type target = *std::max_element(positions.begin(), positions.end());

        for(size_type i = static_cast<size_type>(positions.size()); --i >= 0;){
            if(positions[i] == target){
                this->operator[](target) = *(first + i);
                this->indices.emplace_back(i);
                --target;
            }
        }

        std::reverse(this->indices.begin(), this->indices.end());
    }
};


} // namespace lib
