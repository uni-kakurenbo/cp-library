#pragma once


#include <iterator>
#include <ranges>
#include <concepts>
#include <algorithm>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace uni {


template<class Size = internal::size_t>
struct next_combination {
    using size_type = Size;

  private:
    size_type _size;

  public:
    next_combination(const size_type size) : _size(size) {};

    auto size() noexcept(NO_EXCEPT) { return this->_size; }

    template<std::ranges::bidirectional_range R>
    auto operator()(R&& range) const noexcept(NO_EXCEPT) { return this->operator()(ALL(range)); }

    template<std::bidirectional_iterator I, std::sentinel_for<I> S>
    auto operator()(I first, S last) const noexcept(NO_EXCEPT) {
        auto subset = std::ranges::next(first, this->size());

        if(first == last || first == subset || last == subset) return false;

        auto src = subset;
        while (first != src) {
            --src;

            if (*src < *std::ranges::prev(last)) {
                auto dest = subset;
                while (*src >= *dest) ++dest;

                std::ranges::iter_swap(src, dest);
                std::ranges::rotate(std::ranges::next(src), std::ranges::next(dest), last);
                std::ranges::rotate(subset, std::ranges::next(subset, std::ranges::distance(dest, last) - 1), last);

                return true;
            }
        }

        std::ranges::rotate(first, subset, last);

        return false;
    }
};


}  // namespace uni
