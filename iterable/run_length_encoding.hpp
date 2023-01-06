#pragma once

#include <iterator>
#include <vector>
#include <utility>

#include "internal/types.hpp"

namespace lib {

template<class T, class container = std::vector<std::pair<T,internal::size_t>>>
struct run_length : container {
    run_length() {}
    template<class I> run_length(const I first, const I last) {
        this->clear();
        typename C::value_type::second_type cnt = 0;
        for(I itr=first, prev=itr; itr!=last; ++itr) {
            if(*prev != *itr) this->emplace_back(*prev, cnt), cnt = 0;
            ++cnt;
            prev = itr;
        }
        this->emplace_back(*prev(last), cnt);
    }
};

} // namespace lib
