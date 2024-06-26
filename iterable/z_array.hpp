#pragma once


#include <concepts>
#include <ranges>


#include "internal/types.hpp"

#include "adaptor/valarray.hpp"


namespace uni {


// Thanks to: atcoder::z_algorithm
template<class SizeType = internal::size_t, class Container = valarray<SizeType>>
struct z_array : Container {
    using size_type = SizeType;

    template<std::input_iterator I, std::sentinel_for<I> S>
    z_array(I first, S last) : Container(std::ranges::distance(first, last), {}) {
        const size_type n = static_cast<size_type>(std::ranges::distance(first, last));
        if(n == 0) return;
        for(size_type i = 1, j = 0; i < n; ++i) {
            size_type& k = this->operator[](i);
            k = (j + this->operator[](j) <= i) ? 0 : std::ranges::min(j + this->operator[](j) - i, this->operator[](i - j));
            while(i + k < n and first[k] == first[i + k]) ++k;
            if(j + this->operator[](j) < i + this->operator[](i)) j = i;
        }
        *this->begin() = n;
    }

    template<std::ranges::input_range R>
    explicit z_array(R&& range) : z_array(ALL(range)) {}
};


} // namespace uni
