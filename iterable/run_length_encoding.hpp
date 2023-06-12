#pragma once


#include <iterator>
#include <vector>
#include <utility>

#include "internal/types.hpp"
#include "internal/types.hpp"


namespace lib {

template<class T, class container = std::vector<std::pair<T,internal::size_t>>>
struct run_length : container {
    explicit run_length() noexcept(NO_EXCEPT) {}

    template<class I>
    explicit run_length(const I first, const I last) noexcept(NO_EXCEPT) {
        this->clear();
        typename container::value_type::second_type cnt = 0;
        for(I itr=first, prev=itr; itr!=last; ++itr) {
            if(*prev != *itr) this->emplace_back(*prev, cnt), cnt = 0;
            ++cnt;
            prev = itr;
        }
        this->emplace_back(*prev(last), cnt);
    }
};

template<class I>
explicit run_length(const I, const I) -> run_length<typename std::iterator_traits<I>::value_type>;


} // namespace lib
