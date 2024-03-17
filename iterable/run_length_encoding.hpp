#pragma once


#include <iterator>
#include <vector>
#include <utility>
#include <ranges>


#include "snippet/aliases.hpp"
#include "internal/types.hpp"

#include "adaptor/vector.hpp"


namespace lib {

template<class T, class container = vector<std::pair<T,internal::size_t>>>
struct run_length : container {
    run_length() noexcept(NO_EXCEPT) = default;

    template<std::input_iterator I, std::sentinel_for<I> S>
    run_length(I first, S last) noexcept(NO_EXCEPT) {
        this->clear();
        typename container::value_type::second_type cnt = 0;
        for(I itr=first, prev=itr; itr!=last; ++itr) {
            if(*prev != *itr) this->emplace_back(*prev, cnt), cnt = 0;
            ++cnt;
            prev = itr;
        }
        this->emplace_back(*std::ranges::prev(last), cnt);
    }

    template<std::ranges::input_range R>
    explicit run_length(R&& range) : run_length(ALL(range)) {};
};

template<std::input_iterator I, std::sentinel_for<I> S>
run_length(I, S) -> run_length<std::iter_value_t<I>>;

template<std::ranges::input_range R>
explicit run_length(R&& range) -> run_length<std::ranges::range_value_t<R>>;


} // namespace lib
