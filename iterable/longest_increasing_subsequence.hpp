#pragma once


#include <vector>
#include <algorithm>
#include <iterator>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "grid.hpp"


namespace lib {


template<bool STRICT, class T = i64, class container = std::vector<T>>
struct lis : container {
    using size_type = typename internal::size_t;

    std::vector<int> indices;

    lis() noexcept(DEV_ENV) {}
    template<class I> lis(const I first, const I last) noexcept(DEV_ENV) {
        std::vector<size_type> positions(std::distance(first, last), -1);

        size_type pos = 0;
        for(auto itr=first; itr!=last; ++pos, ++itr) {
            typename container::iterator bound;

            if constexpr(STRICT) bound = std::lower_bound(std::begin(*this), std::end(*this), *itr);
            else bound = std::upper_bound(std::begin(*this), std::end(*this), *itr);

            positions[pos] = std::distance(std::begin(*this), bound);

            if(std::end(*this) == bound) this->emplace_back(*itr);
            else *bound = *itr;
        }

        size_type target = *std::max_element(positions.begin(), positions.end());

        for(size_type i = positions.size(); --i >= 0;){
            if(positions[i] == target){
                (*this)[target] = *(first + i);
                this->indices.emplace_back(i);
                --target;
            }
        }

        std::reverse(this->indices.begin(), this->indices.end());
    }
};


} // namespace lib
