#pragma once


#include <iterator>
#include <vector>
#include <utility>
#include <ranges>

#include "internal/types.hpp"
#include "internal/types.hpp"

#include "adapter/vector.hpp"


namespace lib {

template<class T, class container = vector<std::pair<T,internal::size_t>>>
struct run_length : container {
    explicit run_length() noexcept(NO_EXCEPT) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    explicit run_length(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        typename container::value_type::second_type cnt = 0;
        for(I itr=first, prev=itr; itr!=last; ++itr) {
            if(*prev != *itr) this->emplace_back(*prev, cnt), cnt = 0;
            ++cnt;
            prev = itr;
        }
        this->emplace_back(*std::ranges::prev(last), cnt);
    }
};

template<std::input_iterator I, std::sentinel_for<I> S>
explicit run_length(I, S) -> run_length<typename std::iterator_traits<I>::value_type>;


} // namespace lib
